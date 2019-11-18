QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../src

SOURCES +=  \
    ../src/configuration.cpp \
    ../src/logcontainer.cpp \
    ../src/logline.cpp \
    ../src/logloader.cpp \
    configuration_test.cpp \
    logcontainer_test.cpp \
    test_main.cpp


HEADERS += \
    ../src/configuration.h \
    ../src/logcontainer.h \
    ../src/logline.h \
    ../src/logloader.h \
    configuration_test.h \
    logcontainer_test.h


QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
