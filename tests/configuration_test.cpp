#include "configuration_test.h"

ConfigurationTest::ConfigurationTest()
{

}

ConfigurationTest::~ConfigurationTest()
{
}

void ConfigurationTest::test_emptyConfig()
{
    c.loadCfg(nullptr);
    QVERIFY(c.getNodeCount() == Configuration::default_nodes);
    QVERIFY(c.getLogPath() == "log.txt");
}


