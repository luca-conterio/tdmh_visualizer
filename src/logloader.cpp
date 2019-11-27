#include "logloader.h"
#include "configuration.h"
#include <fstream>
#include <vector>
#include <memory>
#include <QFileSystemWatcher>
#include <QEventLoop>
#include <QThread>
LogLoader::LogLoader(const std::string &path, Configuration::MODE mode, const std::shared_ptr<LogContainer> &logC, const std::shared_ptr<TSQueue> &queue):
    path(path),mode(mode),logC(logC),queue(queue)
{

}

void LogLoader::load()
{
    if(this->isRunning())return;

    this->start();
}

void LogLoader::stop()
{
    if(queue!=nullptr)queue->breakQueue();
    this->requestInterruption();
}

void LogLoader::loadBatch(const std::string& path, const std::shared_ptr<LogContainer>& logC,const std::shared_ptr<TSQueue>& queue,bool callRTAfter)
{
    const std::string& fileName(path);
    std::string delimeter=";";
    std::ifstream file(fileName);
    unsigned int lineN=0;
    clock_t begin = clock();
    auto line= std::string();
    while (!this->isInterruptionRequested() && getline(file, line)&& !queue->isBroken())
    {
        lineN++;
        line.shrink_to_fit();
        //std::cout<<"Line: "<<*line<<std::endl;
        queue->push(new std::string(line));


        logC->process(lineN,line);
    }

    clock_t end = clock();
    double elapsedSeconds = double(end - begin) / CLOCKS_PER_SEC;

    std::cout << "Batch loading done "<<elapsedSeconds<<"s"<<std::endl;
    if(callRTAfter){
        loadRT(lineN,file,logC,queue);
    }
    file.close();
    queue->breakQueue();

}

void LogLoader::loadStat(const std::string &path, const std::shared_ptr<LogContainer>&logC,const std::shared_ptr<TSQueue>&queue)
{

}

void LogLoader::loadRT(unsigned int lineN,std::ifstream &file, const std::shared_ptr<LogContainer>& logC, const std::shared_ptr<TSQueue>&queue)
{
    queue->waitEmpty();
    queue->toRT();

    std::cout<<"Beginning RTime file watching"<<std::endl;

    std::string line;
    while (this->isInterruptionRequested())//This QThread will get terminated when the window is called
    {
        while (std::getline(file, line)) {
            lineN++;
            logC->process(lineN,line);
            queue->push(new std::string(line));
        }
        if (!file.eof()) break;
        file.clear();
        std::this_thread::sleep_for(std::chrono::milliseconds(this->realTimeSleep));
    }

}

void LogLoader::run()
{
    switch(mode){
        case Configuration::MODE::BATCH:
            loadBatch(path,logC,queue,false);

        break;
        case Configuration::MODE::RTIME:
            loadBatch(path,logC,queue,true);
        break;
        case Configuration::MODE::STAT:
            loadStat(path,logC,queue);
        break;

    }
}
