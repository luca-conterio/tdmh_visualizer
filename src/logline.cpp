#include "logline.h"

#include <utility>

LogLine::LogLine(unsigned int nodeId, unsigned int lineN, boost::dynamic_bitset<> strongMask, boost::dynamic_bitset<> weakMask):nodeId(nodeId),lineN(lineN),strongMask(std::move(strongMask)),weakMask(std::move(weakMask)){};
