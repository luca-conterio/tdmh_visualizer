#include "logline.h"

unsigned int LogLine::getNodeId() const
{
    return nodeId;
}

unsigned int LogLine::getLineN() const
{
    return lineN;
}

const std::vector<bool> *LogLine::getStrongMask() const
{
    return strongMask;
}

const std::vector<bool> *LogLine::getWeakMask() const
{
    return weakMask;
}

LogLine::LogLine(unsigned int nodeId, unsigned int lineN, std::vector<bool> *strongMask, std::vector<bool> *weakMask):nodeId(nodeId),lineN(lineN),strongMask(strongMask),weakMask(weakMask){}

LogLine::LogLine(const LogLine &oth):nodeId(oth.nodeId),lineN(oth.lineN),strongMask(new std::vector<bool>(*oth.strongMask)),weakMask(new std::vector<bool>(*oth.weakMask))
{

}

LogLine::~LogLine()
{
    delete strongMask;
    delete weakMask;
}
