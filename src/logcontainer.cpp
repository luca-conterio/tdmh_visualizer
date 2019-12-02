#include "logcontainer.h"
#include <iostream>
#include <algorithm>
#include <regex>

unsigned int LogContainer::getTempThresh() const
{
    return tempThresh;
}

std::unique_ptr<std::vector<bool> > LogContainer::toBoolVec(const std::vector<char> &mask) const
{
    auto out=std::make_unique<std::vector<bool>>(mask.size());
    for(unsigned long i=0;i<out->size();i++){
        out->at(i)=(mask[i]=='1');
    }
    return out;
}

LogContainer::LogContainer(const unsigned int initSize)
{
    for(unsigned int i=0;i<initSize;i++){
        store.push_back(std::make_unique<std::vector<LogLine>>());
    }
}

void LogContainer::addLine(const unsigned int nodeId, const unsigned int lineN, const std::vector<char> &strong, const std::vector<char> &weak)
{
    std::lock_guard<std::mutex> lck(storeMutex);

    if(nodeId>=store.size()){
        //If node was not accounted for, grow the store size to account for it
        for(unsigned long i=store.size();i<=nodeId;i++){
            store.push_back(std::make_unique<std::vector<LogLine>>());
        }
    }

    LogLine a(nodeId,lineN,toBoolVec(strong),toBoolVec(weak));

    store[nodeId]->push_back(a);
}

LogLine LogContainer::findLine(const unsigned int nodeId, const unsigned int maxLine)
{
    std::lock_guard<std::mutex> lck(storeMutex);

    if(nodeId>=store.size()){
        std::cout << "Invalid node id "<<nodeId <<std::endl;
        //Return generic line
        return {nodeId,0,std::make_unique<std::vector<bool>>(), std::make_unique<std::vector<bool>>()};
    }

    //Find first that is strict greater than line number
    auto found= std::upper_bound (store[nodeId]->begin(), store[nodeId]->end(), maxLine, LogLine::comparator);

    if( (found==store[nodeId]->begin()) ){//No line is before given line number => return a generic line
        return {nodeId,0,std::make_unique<std::vector<bool>>(),std::make_unique<std::vector<bool>>()};
    }
    if((found==store[nodeId]->end())){ //All lines are before=> return last
        return store[nodeId]->back();
    }
    return *(found-1);//Return first that is not greater
}

unsigned int LogContainer::getSize()
{
    return static_cast<unsigned int> (store.size());
}

void LogContainer::process(const unsigned int lineN,const std::string &line)
{
    //[U] Topo 000: [0000000000000000][0000000000000000]

    const auto res = std::mismatch(prefix.begin(), prefix.end(), line.begin());
    if (res.first != prefix.end())//Majority of ignored strings can be eliminated just by checking their beginning
    {
      return;
    }

    std::vector <std::string> split;
    std::stringstream sst(line);
    std::string intermediate;

    while(getline(sst, intermediate, ' '))
    {
        split.push_back(intermediate);
    }
    if(split.size()<3)return;

    split[2].pop_back();//Remove ':'
    try {
        //std::cout<<line<<" "<<lineN<<std::endl;
        auto index=static_cast<unsigned int>(std::stoi(split[2]));
        unsigned long umaskSize;
        bool weakPresent= (std::count(split[3].begin(), split[3].end(), '[')>1);

        std::string::iterator it=split[3].begin()+1;
        if(weakPresent){
            umaskSize = (split[3].size()-4)/2;
        }else{
            umaskSize = split[3].size()-3;
        }
        auto maskSize=static_cast<long>(umaskSize);
        std::vector<char> dataS(it,it+maskSize);


        std::vector<char> dataW;
        if(weakPresent){
            it+=maskSize;
            dataW=std::vector<char>(it+2,it+2+maskSize);
        }else{
            dataW=std::vector<char>(dataS.size(),'0');
        }

        //std::cout<<"Parsed s "<<std::string(dataS.begin(), dataS.end())<<std::endl;
        //std::cout<<"Parsed w "<<std::string(dataW.begin(), dataW.end())<<std::endl;

        this->addLine(index,lineN,dataS,dataW);
    } catch (std::invalid_argument&) {
        //Invalid line, ignore
    }

}

void LogContainer::processStat( const std::string &line)
{
    //Timed
    auto ntPos=line.find("NT=");
    if(ntPos!= std::string::npos) {   //tieni ultimo timestamp
        auto numStr=line.substr(ntPos+3);
        try {
            currentTimestamp= std::stoul(numStr);
        } catch (std::invalid_argument&) {

        }
    }else if(line.size()>=7 && line[0]=='[' && line[line.size()-2]==']') {//[0 - 1]
        auto dashPos=line.find('-');
        auto lastDigitRelativePos= (line.size()-1)-dashPos-2;
        try {
            int first= std::stoi(line.substr(1,dashPos-2));
            int second= std::stoi(line.substr(dashPos+2,lastDigitRelativePos) );
            currentLinks.insert({first,second});

        } catch (std::invalid_argument&) {

        }
    }else if(line.find("[SC] Begin Topology") != std::string::npos) {
        if(firstTimestamp>currentTimestamp) {
            firstTimestamp=currentTimestamp;    //inizializza current time
        } else {
            for(auto p: currentLinks) { //aggiorna ogni link dicendo che è durato dall'ultimo Begin Topology a questo
                if(p.first>maxNode)updateMatSize(p.first);
                if(p.second>maxNode)updateMatSize(p.second);
                //std::cout << p.first<<" "<<p.second<<" "<<maxNode<<std::endl;
                //std::cout <<timedMat.size()<< " " <<timedMat[p.first].size()<<std::endl;
                timedMat[p.first][p.second]+=currentTimestamp-lastTimestamp;
                timedMat[p.second][p.first]+=currentTimestamp-lastTimestamp;

                untimedMat[p.first][p.second]++;
                untimedMat[p.second][p.first]++;
            }
        }
        currentLinks.clear(); //reset link
        lastTimestamp=currentTimestamp;

        emittedTopologies++;
    }


}

void LogContainer::setTempThresh(unsigned int value)
{
    tempThresh = value;
}

void LogContainer::lastLine()
{
    for(auto p: currentLinks) { //Update with last values
        if(p.first>maxNode)updateMatSize(p.first);
        if(p.second>maxNode)updateMatSize(p.second);

        timedMat[p.first][p.second]+=currentTimestamp-lastTimestamp;
        timedMat[p.second][p.first]+=currentTimestamp-lastTimestamp;

        untimedMat[p.first][p.second]++;
        untimedMat[p.second][p.first]++;
    }

    std::cout<<"maxNode is "<<maxNode<<std::endl;
    const auto maxConst=maxNode;
    const double unDiv=emittedTopologies;

    for(unsigned int i=0;i<=maxConst;i++){
        timedPercMat.emplace_back(maxNode+1);
        untimedPercMat.emplace_back(maxNode+1);
        for(unsigned int j=0;j<=maxConst;j++){

            const double num=timedMat[i][j];
            const auto div=static_cast<double>( currentTimestamp-firstTimestamp);

            timedPercMat[i][j]= num/div;

            const double unNum=untimedMat[i][j];
            untimedPercMat[i][j]=unNum/unDiv;
            //std::cout << i<<" "<<j<<" "<<timedPercMat[i][j]<<" " <<num<<" div is "<<div<<std::endl;
        }
    }
}

std::vector<std::vector<double> > LogContainer::getAvail(const bool time)
{
    if(time){
        return timedPercMat;
    }
    return untimedPercMat;
}

void LogContainer::updateMatSize(const unsigned int newCount)
{
    while(timedMat.size()<=newCount)timedMat.emplace_back(newCount);
    while(untimedMat.size()<=newCount)untimedMat.emplace_back(newCount);

    for(unsigned int i=0;i<=newCount;i++){
        while(timedMat[i].size()<=newCount)timedMat[i].push_back(0);
        while(untimedMat[i].size()<=newCount)untimedMat[i].push_back(0);
    }
    this->maxNode=newCount;
}
