#include "logline.h"

unsigned int LogLine::getNodeId() const
{
    return nodeId;
}

unsigned int LogLine::getLineN() const
{
    return lineN;
}

bool LogLine::getStrongMask(unsigned long x) const
{
    return (x<strongMask->size())?strongMask->at(x):false;
}

bool LogLine::getWeakMask(unsigned long x) const
{
    return (x<weakMask->size())?weakMask->at(x):false;
}

unsigned long LogLine::getWeakSize() const
{
    return weakMask->size();
}

unsigned long LogLine::getStrongSize() const
{
    return strongMask->size();
}

LogLine::LogLine(unsigned int nodeId, unsigned int lineN, std::unique_ptr<std::vector<bool>>strongMask, std::unique_ptr<std::vector<bool>>weakMask):
    nodeId(nodeId),lineN(lineN),strongMask(std::move(strongMask)),weakMask(std::move(weakMask)){}

LogLine::LogLine(const LogLine &oth):nodeId(oth.nodeId),lineN(oth.lineN),strongMask(new std::vector<bool>(*oth.strongMask)),weakMask(new std::vector<bool>(*oth.weakMask))
{

}
