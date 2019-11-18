#include "configuration.h"
#include <fstream>

Configuration::Configuration()
= default;

void Configuration::loadCfg(char *ptr)
{
    //TODO load from file
}

int Configuration::getNodeCount() const
{
    return this->node_count;
}

std::string Configuration::getLogPath() const
{
    return this->log_path;
}
