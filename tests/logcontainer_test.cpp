
#include "logcontainer_test.h"
#include <vector>
logcontainerTest::logcontainerTest()
{
}

logcontainerTest::~logcontainerTest()
{

}

void logcontainerTest::test_initSize(){
    unsigned int initSize =25;
    LogContainer logC(initSize);
    QVERIFY(logC.getSize() == initSize);
}

void logcontainerTest::test_addLineIncreaseSize()
{
    unsigned int initSize =2;
    LogContainer logC(initSize);
    std::string str = "01010";
    std::vector<char> data(str.begin(), str.end());
    logC.addLine(initSize+2,100,data,data);
    QVERIFY(logC.getSize() == initSize+3);
}

void logcontainerTest::test_lineAdded()
{
    unsigned int initSize =2;
    LogContainer logC(initSize);
    std::string str = "01010";
    std::vector<char> data(str.begin(), str.end());
    logC.addLine(initSize+2,100,data,data);
    QVERIFY(logC.findLine(initSize+2,99).lineN==0);
    QVERIFY(logC.findLine(initSize+2,100).lineN==100);
    QVERIFY(logC.findLine(initSize+2,101).lineN==100);
    QVERIFY(logC.findLine(25,0).lineN==0);
}

void logcontainerTest::test_multipleLinesAdded()
{
    unsigned int initSize =2;
    LogContainer logC(initSize);
    std::string str = "01010";
    std::vector<char> data(str.begin(), str.end());
    logC.addLine(initSize+2,100,data,data);
    logC.addLine(initSize+2,200,data,data);
    QVERIFY(logC.findLine(initSize+2,99).lineN==0);
    QVERIFY(logC.findLine(initSize+2,100).lineN==100);
    QVERIFY(logC.findLine(initSize+2,101).lineN==100);
    QVERIFY(logC.findLine(initSize+2,199).lineN==100);
    QVERIFY(logC.findLine(initSize+2,200).lineN==200);
    QVERIFY(logC.findLine(initSize+2,201).lineN==200);
}


