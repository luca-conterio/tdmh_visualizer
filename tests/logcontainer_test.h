#ifndef LOGCONTAINER_TEST_H
#define LOGCONTAINER_TEST_H


#include <QtTest>
#include <QCoreApplication>
#include <logcontainer.h>
class logcontainerTest : public QObject
{
    Q_OBJECT

public:
    logcontainerTest();
    ~logcontainerTest();

private:
private slots:
    void test_initSize();
    void test_addLineIncreaseSize();
    void test_lineAdded();
    void test_multipleLinesAdded();
    /*void initTestCase(){
        qDebug("Hello init test deb");
    }*/

};

#endif // LOGCONTAINER_TEST_H
