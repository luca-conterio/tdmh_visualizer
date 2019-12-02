#include "logloader.h"
#include "configuration.h"
#include <fstream>
#include <vector>
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
    this->terminate=true;
}

void LogLoader::loadBatch(const std::string& path, const std::shared_ptr<LogContainer>& logC,const std::shared_ptr<TSQueue>& queue,const bool callRTAfter)
{
    const std::string& fileName(path);
    std::ifstream file(fileName);
    unsigned int lineN=0;
    const clock_t begin = clock();

    auto line= std::string();
    while (!terminate && getline(file, line)&& !queue->isBroken())
    {
        lineN++;
        line.shrink_to_fit();
        //std::cout<<"Line: "<<*line<<std::endl;
        queue->push(new std::string(line));
        logC->process(lineN,line);
    }

    const clock_t end = clock();
    const auto elapsedSeconds = double(end - begin) / CLOCKS_PER_SEC;

    std::cout << "Batch loading done "<<elapsedSeconds<<"s"<<std::endl;

    if(callRTAfter){
        loadRT(lineN,file,logC,queue);
    }
    file.close();
    queue->breakQueue();

}

inline void printAvail(const bool timed,const std::shared_ptr<LogContainer>&logC,const std::shared_ptr<TSQueue>&queue){
    unsigned printedCount=1;

    const auto matrix=logC->getAvail(timed);
    const auto nodeCount=matrix.size();
    const int hundred=100;

    for(size_t i=0;i<nodeCount;i++){
        for(size_t j=i;j<nodeCount;j++){
            if(matrix[i][j]>0.0){
                std::string *text= new std::string("["
                                                +std::to_string(i)
                                                +" - "
                                                +std::to_string(j)
                                                +"] = "
                                                +std::to_string(matrix[i][j]*hundred)
                                                +"%"
                                                );
                queue->push(text);
                printedCount++;
            }
        }
    }

    queue->push(new std::string());
    printedCount++;

    if(timed){
        logC->setTempThresh(printedCount);
    }
}

void LogLoader::loadStat(const std::string &path, const std::shared_ptr<LogContainer>&logC,const std::shared_ptr<TSQueue>&queue)
{
    const std::string& fileName(path);
    std::ifstream file(fileName);
    unsigned int lineN=0;

    auto line= std::string();
    while (!terminate && getline(file, line)&& !queue->isBroken())
    {
        lineN++;
        line.shrink_to_fit();
        //std::cout<<"Line: "<<*line<<std::endl;

        logC->processStat(line);
    }
    std::cout<<"Finished building statistics"<<std::endl;
    file.close();

    //Signal to Log Container that the file is over
    logC->lastLine();

    //Output stats to text
    queue->push(new std::string("Timed availabilities"));
    printAvail(true,logC,queue);
    queue->push(new std::string("Non Timed availabilities"));
    printAvail(false,logC,queue);

    queue->breakQueue();
}

void LogLoader::loadRT(unsigned int lineN,std::ifstream &file, const std::shared_ptr<LogContainer>& logC, const std::shared_ptr<TSQueue>&queue)
{
    queue->waitEmpty();
    queue->toRT();

    std::cout<<"Beginning RTime file watching"<<std::endl;

    std::string line;
    while (!terminate && !queue->isBroken())//Will get terminated when stop() is called
    {
        while (std::getline(file, line)) {
            lineN++;
            logC->process(lineN,line);
            queue->push(new std::string(line));
        }
        file.clear(); //Clear eof status and wait for a new line
        std::this_thread::sleep_for(std::chrono::milliseconds(this->realTimeSleep));
    }
    std::cout <<"Ending RTime file watching"<<std::endl;

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
