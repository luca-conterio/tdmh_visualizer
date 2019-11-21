#include "logcontainer.h"
#include <iostream>
#include <algorithm>
#include <regex>
std::unique_ptr<std::vector<bool> > LogContainer::toBoolVec(const std::vector<char> &mask) const
{
    auto out=std::make_unique<std::vector<bool>>(mask.size());
    for(unsigned long i=0;i<out->size();i++){
        out->at(i)=(mask[i]=='1');
    }
    return out;
}

LogContainer::LogContainer(unsigned int initSize)
{
    for(unsigned int i=0;i<initSize;i++){
        store.push_back(std::make_unique<std::vector<LogLine>>());
    }
}

void LogContainer::addLine(unsigned int nodeId, unsigned int lineN, const std::vector<char> &strong, const std::vector<char> &weak)
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

LogLine LogContainer::findLine(unsigned int nodeId, unsigned int maxLine)
{
    std::lock_guard<std::mutex> lck(storeMutex);
    if(nodeId>=store.size()){
        std::cout << "Invalid node id "<<nodeId <<"\n";
        return {nodeId,0,std::make_unique<std::vector<bool>>(), std::make_unique<std::vector<bool>>()};
    }
    auto found= std::upper_bound (store[nodeId]->begin(), store[nodeId]->end(), maxLine, LogLine::comparator);

    if( (found==store[nodeId]->begin()) ){
        return {nodeId,0,std::make_unique<std::vector<bool>>(),std::make_unique<std::vector<bool>>()};
    }
    if((found==store[nodeId]->end())){
        return store[nodeId]->back();
    }
    return *(found-1);
}

unsigned int LogContainer::getSize()
{
    return static_cast<unsigned int> (store.size());
}

void LogContainer::process(unsigned int lineN,const std::string &line)
{

    //Regex works but is Really Really slow
    /*bool initWeak=false;
    std::sregex_iterator declare_begin;
    auto declare_end = std::sregex_iterator();

    //[U] Topo 000: [0000000000000000][0000000000000000]
    std::regex weakP(R"(^\[U\] Topo ([0-9][0-9][0-9]): \[([0-9]+)\]\[([0-9]+)\])");

    declare_begin = std::sregex_iterator(line.begin(), line.end(), weakP);
    if(declare_begin==declare_end){
        initWeak=true;
        std::regex strongP(R"(^\[U\] Topo ([0-9][0-9][0-9]): \[([0-9]+)\])");
        declare_begin = std::sregex_iterator(line.begin(), line.end(), strongP);
        if(declare_begin==declare_end)return;
    }

    for (std::sregex_iterator i = declare_begin; i != declare_end; ++i) {
        std::smatch match = *i;
        //std::cout << match[1] << " is " << match[2] << " "<<match[3]<<'\n';

        std::string strong=match[2].str();
        std::string weak;
        if(initWeak){
            weak=std::string(strong.size(), '0');
        }else{
            weak=match[3].str();
        }
        unsigned int index=static_cast<unsigned int>(std::stoi(match[1].str()));


        std::vector<char> dataS(strong.begin(),strong.end());
        std::vector<char> dataW(weak.begin(),weak.end());

        this->addLine(index,lineN,dataS,dataW);

    }*/

    //[U] Topo 000: [0000000000000000][0000000000000000]
    std::vector <std::string> split;
    std::stringstream sst(line);
    std::string intermediate;

    while(getline(sst, intermediate, ' '))
    {
        split.push_back(intermediate);
    }
    if(split[0]=="[U]" && split[1]=="Topo"){
        split[2].pop_back();//Remove :
        try {
            unsigned int index=static_cast<unsigned int>(std::stoi(split[2]));
            size_t maskSize= (split[3].size()-4)/2;
            std::string strong=split[3].substr(1,maskSize);
            std::string weak=split[3].substr(maskSize+3,maskSize);

            //std::cout<<line<<" "<<lineN<<"\n";
            /* std::cout<<"Read "<<strong <<"  "<<weak<<" \n";*/

            std::string::iterator it=split[3].begin()+1;
            std::vector<char> dataS(it,it+(split[3].size()-4)/2);
            it+=(split[3].size()-4)/2;
            std::vector<char> dataW(it+2,it+2+(split[3].size()-4)/2);

            /*std::cout<<"Parsed s"<<std::string(dataS.begin(), dataS.end())<<" \n";
            std::cout<<"Parsed w "<<std::string(dataW.begin(), dataW.end())<<" \n";*/

            this->addLine(index,lineN,dataS,dataW);
        } catch (std::invalid_argument&) {
            //Invalid line, ignore
        }

    }

}

/*LogContainer::~LogContainer()
{
    for(auto v :store) {
        delete v;
    }
}*/
