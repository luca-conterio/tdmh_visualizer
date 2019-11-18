#include "logcontainer.h"
#include <iostream>
#include <algorithm>
std::vector<bool> *LogContainer::toBoolVec(std::vector<char> mask) const
{
    std::vector<bool>* out=new std::vector<bool>(mask.size());
    for(unsigned long i=0;i<out->size();i++){
        out->at(i)=(mask[i]=='1');
    }
    return out;
}

LogContainer::LogContainer(unsigned int initSize)
{
    for(unsigned int i=0;i<initSize;i++){
        store.push_back(new std::vector<LogLine>);
    }
}

void LogContainer::addLine(unsigned int nodeId, unsigned int lineN, std::vector<char> strong, std::vector<char> weak)
{
    std::lock_guard<std::mutex> lck(storeMutex);
    if(nodeId>=store.size()){
        //If node was not accounted for, grow the store size to account for it
        for(unsigned long i=store.size();i<=nodeId;i++){
            store.push_back(new std::vector<LogLine>);
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
        return LogLine(nodeId,0,new std::vector<bool>(), new std::vector<bool>());
    }
    auto found= std::upper_bound (store[nodeId]->begin(), store[nodeId]->end(), maxLine, LogLine::comparator);

    if( (found==store[nodeId]->begin()) ){
        return LogLine(nodeId,0,new std::vector<bool>(),new std::vector<bool>());
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

LogContainer::~LogContainer()
{
    for(auto v :store) {
        delete v;
    }
}
