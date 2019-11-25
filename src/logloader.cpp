#include "logloader.h"
#include "configuration.h"
#include <fstream>
#include <vector>
#include <memory>
void LogLoader::load(const std::string& path, Configuration::MODE mode, const std::shared_ptr<LogContainer>& logC, const std::shared_ptr<TSQueue>& queue)
{
    this->queue=queue;
    if(loaderThread!=nullptr){return;}
    switch(mode){
        case Configuration::MODE::BATCH:
            loaderThread=std::make_unique<std::thread>(loadBatch,path,logC,queue);
        break;
        case Configuration::MODE::RTIME:
            loaderThread=std::make_unique<std::thread>(loadRT,path,logC,queue);
        break;
        case Configuration::MODE::STAT:
            loaderThread=std::make_unique<std::thread>(loadStat,path,logC,queue);
        break;

    }
}

void LogLoader::stop()
{
    if(queue!=nullptr)queue->breakQueue();
    loaderThread->join();
}

void LogLoader::loadBatch(const std::string& path, const std::shared_ptr<LogContainer>& logC,const std::shared_ptr<TSQueue>& queue)
{
    const std::string& fileName(path);
    std::string delimeter=";";
    std::ifstream file(fileName);
    unsigned int lineN=0;

    auto* line=new std::string;
    while (getline(file, *line)&& !queue->isBroken())
    {
        lineN++;
        line->shrink_to_fit();
        //std::cout<<"Line: "<<*line<<"\n";
        queue->push(line);
        logC->process(lineN,*line);
        line=new std::string;
    }
    file.close();
    queue->breakQueue();
}

void LogLoader::loadStat(const std::string& path, const std::shared_ptr<LogContainer>&logC,const std::shared_ptr<TSQueue>&queue)
{

}

void LogLoader::loadRT(const std::string& path, const std::shared_ptr<LogContainer>& logC, const std::shared_ptr<TSQueue>&queue)
{

}
