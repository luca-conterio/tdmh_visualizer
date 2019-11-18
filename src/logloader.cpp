#include "logloader.h"
#include "configuration.h"
#include <fstream>
#include <vector>
#include <iostream>
LogLoader::LogLoader()
{

}

void LogLoader::load(std::string path, Configuration::MODE mode, LogContainer *logC, TSQueue *queue)
{
    //this->gui=gui;
    if(loaderThread!=nullptr){return;}
    switch(mode){
        case Configuration::MODE::BATCH:
            loaderThread=new std::thread(loadBatch,path,logC,queue);
        break;
        case Configuration::MODE::RTIME:
            loaderThread=new std::thread(loadRT,path,logC,queue);
        break;
        case Configuration::MODE::STAT:
            loaderThread=new std::thread(loadStat,path,logC,queue);
        break;

    }
}

void LogLoader::loadBatch(const std::string path, LogContainer *logC,TSQueue *queue)
{
    std::string fileName(path);
    std::string delimeter=";";
    std::ifstream file(fileName);


    std::string *line=new std::string;
    while (getline(file, *line))
    {
        //std::cout<<"Line: "<<*line<<"\n";
        queue->push(*line);
    }
    file.close();
}

void LogLoader::loadStat(const std::string path, LogContainer *logC,TSQueue *queue)
{

}

void LogLoader::loadRT(const std::string path, LogContainer *logC,TSQueue *queue)
{

}
