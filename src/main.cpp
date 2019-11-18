#include "configuration.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <iostream>
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
        cfg.loadCfg(argv[1]);
    }

    std::cout<< "Loaded "<<cfg.getNodeCount()<<" nodes\n";




    std::cout.flush();



    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return QGuiApplication::exec();
}
