#include <QtTest>
#include <QCoreApplication>

// add necessary includes here

class testcase : public QObject
{
    Q_OBJECT

public:
    testcase();
    ~testcase();

private slots:
    void test_case1();

};

testcase::testcase()
{

}

testcase::~testcase()
{

}

void testcase::test_case1()
{

}

QTEST_MAIN(testcase)

#include "tst_testcase.moc"
