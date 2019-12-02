#include "logloader.h"
#include "configuration.h"
#include <fstream>
#include <vector>
#include <tuple>
#include <queue>
#include <memory>
#include <QFileSystemWatcher>
#include <QEventLoop>
#include <QThread>

/*!
 * \brief The CompareAvail struct is used to compare two triples for the priority queue
 */
struct CompareAvail{
    /*!
     * \brief operator () implements less than between two triples, comparing the double value
     * \param l left triple
     * \param r right triple
     * \return l[2]<r[2]
     */
    bool operator()(const std::tuple<size_t,size_t,double>& l, const std::tuple<size_t,size_t,double> r)
    {
        return std::get<2>(l) < std::get<2>(r);
    }
};

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

void LogLoader::loadBatch(const std::string& path, const std::shared_ptr<LogContainer>& logC,const std::shared_ptr<TSQueue>& queue,bool callRTAfter)
{
    const std::string& fileName(path);
    const std::string delimeter=";";
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
    double elapsedSeconds = double(end - begin) / CLOCKS_PER_SEC;

    std::cout << "Batch loading done "<<elapsedSeconds<<"s"<<std::endl;
    if(callRTAfter){
        loadRT(lineN,file,logC,queue);
    }
    file.close();
    queue->breakQueue();

}

inline void printAvail(bool timed,const std::shared_ptr<LogContainer>&logC,const std::shared_ptr<TSQueue>&queue){
    unsigned printedCount=1;
    auto matrix=logC->getAvail(timed);
    const auto nodeCount=matrix.size();
    const int hundred=100;
    //std::priority_queue<std::tuple<size_t,size_t,double>,std::vector<std::tuple<size_t,size_t,double>>,CompareAvail> list;
    for(size_t i=0;i<nodeCount;i++){
        for(size_t j=i;j<nodeCount;j++){
            //list.push({i,j,matrix[i][j]});
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
    /*while(!list.empty()){
        auto triple=list.top();
        if(std::get<2>(triple)>0.0){
            std::string *text= new std::string("["
                                            +std::to_string(std::get<0>(triple))
                                            +" - "
                                            +std::to_string(std::get<1>(triple))
                                            +"] = "
                                            +std::to_string(std::get<2>(triple))
                                            +"%"
                                            );
            queue->push(text);
            printedCount++;
        }
        list.pop();
    }*/
    queue->push(new std::string());
    printedCount++;

    if(timed){
        logC->setTempThresh(printedCount);
    }
}

void LogLoader::loadStat(const std::string &path, const std::shared_ptr<LogContainer>&logC,const std::shared_ptr<TSQueue>&queue)
{
    const std::string& fileName(path);
    const std::string delimeter=";";
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

    //Output stats to text
    //Timed
    logC->lastLine();
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
    while (!terminate)//This QThread will get terminated when the window is called
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
