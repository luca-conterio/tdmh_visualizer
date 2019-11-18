#include "configuration.h"
#include <fstream>
#include <vector>
#include <iostream>
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
        if(line[0]=='#')continue;
        std::string opt;
        std::string val;
        size_t pos=line.find('=');
        if(pos==std::string::npos){
            std::cout<< "Invalid config line :"<<line<<"\n";
        }
        opt=line.substr(0,pos);
        val=line.substr(pos+1,std::string::npos);

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
            } catch (std::invalid_argument &e) {
                std::cout <<"Invalid node count: "<<val<<"\n";
            }
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

Configuration::MODE Configuration::getMode()
{
    return this->mode;
}
