#include "logcontainer.h"
#include <iostream>
#include <utility>


boost::dynamic_bitset<> LogContainer::toDynSet(std::vector<char> mask) const
{
    boost::dynamic_bitset<> out(mask.size());
    for(unsigned long i=0;i<out.size();i++){
        out[i]=mask[i];
    }
    return out;
}

LogContainer::LogContainer(int initSize)
{
    for(int i=0;i<initSize;i++){
        store.push_back(new std::vector<LogLine>);
    }
}

void LogContainer::addLine(unsigned int nodeId, unsigned int lineN, std::vector<char> strong, std::vector<char> weak)
{
    if(nodeId>=store.size()){
        std::cout << "Invalid node id "<<nodeId <<"\n";
        return;
    }
    LogLine a(nodeId,lineN,toDynSet(std::move(strong)),toDynSet(std::move(weak)));
    store[nodeId]->push_back(a);
}

LogLine LogContainer::findLine(unsigned int nodeId, unsigned int maxLine) const
{
    if(nodeId>=store.size()){
        std::cout << "Invalid node id "<<nodeId <<"\n";
        return LogLine(nodeId,0,boost::dynamic_bitset<>(),boost::dynamic_bitset<>());
    }
    auto found= std::upper_bound (store[nodeId]->begin(), store[nodeId]->end(), 2, LogLine::comparator);
    if((found==store[nodeId]->end()) && found->lineN>maxLine){
        return LogLine(nodeId,0,boost::dynamic_bitset<>(),boost::dynamic_bitset<>());
    }
    return *found;
}

LogContainer::~LogContainer()
{
    for(auto v :store) {
        delete v;
    }
}
