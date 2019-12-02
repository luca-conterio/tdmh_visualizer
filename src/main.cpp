#include "configuration.h"
#include "logcontainer.h"
#include "logloader.h"

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
        std::cout << "No configuration file given, using defaults"<<std::endl;
        cfg.loadCfg(nullptr);
    }else{
        std::cout<< "Loading file:"<<argv[1]<<std::endl;
        cfg.loadCfg(argv[1]);
    }

    std::cout<< "Loaded "<<cfg.getNodeCount()<<" nodes"<<std::endl;

    std::cout<<"Mode: "<<cfg.getMode()<<std::endl;
    std::cout<<"Log path: "<<cfg.getLogPath()<<std::endl;

    std::cout.flush();


    auto logC=std::make_shared<LogContainer>(static_cast<unsigned int>(cfg.getNodeCount()));
    auto tsq=std::make_shared<TSQueue>();

    auto lld=std::make_shared<LogLoader>(cfg.getLogPath(),cfg.getMode(),logC,tsq);

    QApplication app(argc, argv);

    auto *mW=new MainWindow();
    mW->show();
    mW->setConfig(cfg,logC,lld,tsq);

    lld->load();

    std::cout.flush();
    return QApplication::exec();
}
