#include "configuration.h"
#include "logcontainer.h"
#include "logloader.h"
#include "logvisual.h"

#include <iostream>


#include <QtWidgets>
#include <mainwindow.h>
/*! \file */

/*!
 * \brief main
 * \param argc
 * \param argv
 * \return
 */
int main(int argc, char *argv[])
{
    Configuration cfg;
    if(argc==0){
        std::cout << "No configuration file given, using defaults\n";
        cfg.loadCfg(nullptr);
    }else{
        std::cout<< "Loading file:"<<argv[1]<<"\n";
        cfg.loadCfg(argv[1]);
    }

    std::cout<< "Loaded "<<cfg.getNodeCount()<<" nodes\n";

    std::cout<<"Mode: "<<cfg.getMode()<<"\n";
    std::cout<<"Log path: "<<cfg.getLogPath()<<"\n";

    std::cout.flush();

    LogLoader lld;
    auto * logC=new LogContainer(static_cast<unsigned int>(cfg.getNodeCount()));
    TSQueue tsq;

    QApplication app(argc, argv);

    auto *mW=new MainWindow();
    mW->show();
    mW->setStyleSheet("QMainWindow {background: 'yellow';}");
    mW->setQueue(&tsq);
    mW->setConfig(cfg);
    lld.load(cfg.getLogPath(),cfg.getMode(),logC,&tsq);
    return QApplication::exec();
}
