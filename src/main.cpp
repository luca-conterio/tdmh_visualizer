#include "configuration.h"
#include "logcontainer.h"
#include "logvisual.h"

/*#include <QGuiApplication>
#include <QQmlApplicationEngine>*/
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



    QApplication app(argc, argv);

    /*QPushButton *hello = new QPushButton("This is a simple button");
    hello->resize(100, 30);
    hello->show();*/
    MainWindow *mW=new MainWindow();
    mW->show();
    mW->setStyleSheet("QMainWindow {background: 'yellow';}");


    /*LogVisual editor;
    editor.setWindowTitle(QObject::tr("Code Editor Example"));
    editor.show();*/
    return app.exec();
}
