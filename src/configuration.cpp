#include "configuration.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
Configuration::Configuration()
= default;

void Configuration::loadCfg(char *ptr)
{   
    if(ptr==nullptr)return;
    std::string fileName(ptr);
    std::string delimeter=";";

    std::ifstream file(fileName);


    std::string line;
    while (getline(file, line))
    {
        if(line==""||line[0]=='#')continue;
        std::string opt;
        std::string val;
        size_t pos=line.find('=');
        if(pos==std::string::npos){
            std::cout<< "Invalid config line :"<<line<<"\n";
            continue;
        }
        opt=line.substr(0,pos);
        trim(opt);
        val=line.substr(pos+1,std::string::npos);
        trim(val);

        if(opt=="LOGFILE"){
            log_path=val;
        }else if(opt=="MODE"){
            if(val=="BATCH"){
                mode=BATCH;
            }else if(val=="RTIME"){
                mode=RTIME;
            }else if(val=="STAT"){
                mode=STAT;
            }else{
                std::cout <<"Unrecognized mode: "<<val<<"\n";
            }
        }else if(opt=="NODECOUNT"){
            try {
                node_count=std::stoi(val);
            } catch (std::invalid_argument&) {
                std::cout <<"Invalid node count: "<<val<<"\n";
            }
        }else if(opt=="IMAGE"){
            img_path=val;
        }else if(opt=="NODELIST"){
            std::vector <std::string> triples;
            std::stringstream sst(val);
            std::string intermediate;

            while(getline(sst, intermediate, '('))
            {
                triples.push_back(intermediate);
            }
            for(const auto& t: triples)processTuple(t);

        }else{
            std::cout<<"Unrecognized option "<<opt<<"\n";
        }

    }
    // Close the File
    file.close();
    std::cout <<"Finished loading configs \n";
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


void Configuration::pushNode(int i, int x, int y){
    std::cout <<"Pushing node "<<i<<" "<<x<<" "<<y<<"\n";
}

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

void Configuration::processTuple(const std::string& t)
{
    if(t=="")return;
    std::vector <std::string> triple;
    std::stringstream sst(t);
    std::string intermediate;

    while(getline(sst, intermediate, ','))
    {
        triple.push_back(intermediate);
    }
    if(triple.size()==3){
        try {
            int ni=std::stoi(triple[0]);
            int nx=std::stoi(triple[1]);
            triple[2].pop_back();
            int ny=std::stoi(triple[2]);
            pushNode(ni,nx,ny);
            return;
        } catch (std::invalid_argument) {
            //std::cout <<"Invalid tuple "<<t<<" \n";
        }
    }
    std::cout <<"Invalid tuple "<<t<<" \n";
}

void Configuration::trim(std::string &s)
{
    ltrim(s);
    rtrim(s);
}



