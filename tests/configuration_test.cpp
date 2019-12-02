#include "configuration_test.h"


void ConfigurationTest::test_emptyConfig()
{
    c.loadCfg(nullptr);
    QVERIFY(c.getNodeCount() == Configuration::default_nodes);
    QVERIFY(c.getLogPath() == "log.txt");
}


