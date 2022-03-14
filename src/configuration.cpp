#include "configuration.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

void Configuration::loadCfg(const char *ptr)
{   
    if(ptr==nullptr)return;

    const std::string fileName(ptr);

    std::ifstream file(fileName);

    std::string line;
    while (getline(file, line))
    {
        if(line.empty()||line[0]=='#')continue;
        std::string opt;
        std::string val;

        //Flags
        if(line=="BATCHFIRST"){
            this->batchFirst=true;
            continue;
        }

        //Options
        const size_t pos=line.find('=');
        if(pos==std::string::npos){
            std::cout<< "Invalid config line :"<<line<<std::endl;
            continue;
        }
        opt=line.substr(0,pos);
        trim(opt);
        val=line.substr(pos+1,std::string::npos);
        trim(val);

        if(opt=="LOGFILE"){
            if (!log_path_set) { // otherwise use the command line one
                log_path=val;
            }
        }else if(opt=="MODE"){
            if(val=="BATCH"){
                mode=BATCH;
            }else if(val=="RTIME"){
                mode=RTIME;
            }else if(val=="STAT"){
                mode=STAT;
            }else{
                std::cout <<"Unrecognized mode: "<<val<<std::endl;
            }
        }else if(opt=="NODECOUNT"){
            try {
                node_count=std::stoi(val);
            } catch (std::invalid_argument&) {
                std::cout <<"Invalid node count: "<<val<<std::endl;
            }
        }else if(opt=="IMAGE"){
            img_path=val;
        }else if(opt=="NODELIST"){

            std::stringstream sst(val);
            std::string intermediate;

            //Split on ( and then process each tuple
            while(getline(sst, intermediate, '('))
            {
                processTuple(intermediate);
            }

        }else{
            std::cout<<"Unrecognized option "<<opt<<std::endl;
        }

    }
    // Close the File
    file.close();
    std::cout <<"Finished loading configs"<<std::endl;
}

void Configuration::setLogFilePath(const char *ptr)
{   
    this->log_path = std::string(ptr);
    this->log_path_set = true;
}

int Configuration::getNodeCount() const
{
    return this->node_count;
}

std::string Configuration::getLogPath() const
{
    return this->log_path;
}

std::string Configuration::getImgPath() const
{
    return this->img_path;
}

Configuration::MODE Configuration::getMode() const
{
    return this->mode;
}


std::vector<std::pair<int, int> > Configuration::getNodeList() const
{
    return nodeList;
}

bool Configuration::getBatchFirst() const
{
    return batchFirst;
}

void Configuration::pushNode(const size_t i, const int x, const int y){

    std::cout <<"Pushing node "<<i<<" "<<x<<" "<<y<<std::endl;

    //Fit to new size if bigger
    if(nodeList.size()<=i){
        for(size_t j=nodeList.size();j<=i;j++){
            nodeList.emplace_back(0,0);
        }
    }
    nodeList[i]={x,y};
}

void Configuration::processTuple(const std::string& t)
{
    if(t.empty())return;

    std::vector <std::string> triple;
    std::stringstream sst(t);
    std::string intermediate;

    while(getline(sst, intermediate, ','))
    {
        triple.push_back(intermediate);
    }

    if(triple.size()==3){ // 0,0,0) becomes 0 0 0)
        try {
            triple[2].pop_back();//Remove )
            const int ni=std::stoi(triple[0]);
            const int nx=std::stoi(triple[1]);
            const int ny=std::stoi(triple[2]);

            pushNode(static_cast<size_t>(ni),nx,ny);
            return;
        } catch (std::invalid_argument&) {
            //std::cout <<"Invalid tuple "<<t<<std::endl;
        }
    }
    std::cout <<"Invalid tuple "<<t<<std::endl;
}

static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

void Configuration::trim(std::string &s)
{
    ltrim(s);
    rtrim(s);
}





