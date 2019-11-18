#ifndef CONFIGURATION_TEST_H
#define CONFIGURATION_TEST_H

#include <QtTest>
#include <QCoreApplication>
#include "configuration.h"
class ConfigurationTest : public QObject
{
    Q_OBJECT

public:
    ConfigurationTest();
    ~ConfigurationTest();

private:
    Configuration c;
private slots:
    void test_emptyConfig();

};




#endif // CONFIGURATION_TEST_H
