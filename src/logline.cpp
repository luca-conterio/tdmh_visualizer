#include "logline.h"

LogLine::LogLine(unsigned int nodeId, unsigned int lineN, std::vector<bool> *strongMask, std::vector<bool> *weakMask):nodeId(nodeId),lineN(lineN),strongMask(strongMask),weakMask(weakMask){}

LogLine::LogLine(const LogLine &oth):nodeId(oth.nodeId),lineN(oth.lineN),strongMask(new std::vector<bool>(*oth.strongMask)),weakMask(new std::vector<bool>(*oth.weakMask))
{

};

LogLine::~LogLine()
{
    delete strongMask;
    delete weakMask;
}
