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
    std::cout << "\n##################### \n";
    std::cout << "   TDMH Visualizer \n";
    std::cout << "##################### \n\n";

    Configuration cfg;
    if (argc==1) {
        std::cout << "No configuration file given, using defaults" << std::endl;
        cfg.loadCfg(nullptr);
    }
    else if (argc == 2) {
        std::cout << "Loading configuration file: " << argv[1] << std::endl;
        cfg.loadCfg(argv[1]);
    }
    else if (argc == 3) {
        std::cout << "Loading configuration file: " << argv[1] << std::endl;
        cfg.loadCfg(argv[1]);
        std::cout << "Log file path: " << argv[2] << std::endl;
        cfg.setLogFilePath(argv[2]);
    }
    else if (argc == 4) {
        std::cout << "Loading configuration file: " << argv[1] << std::endl;
        cfg.loadCfg(argv[1]);
        std::cout << "Log file path: " << argv[2] << std::endl;
        cfg.setLogFilePath(argv[2]);
        cfg.setMode(argv[3]);
    }

    std::cout << "Loaded " << cfg.getNodeCount() << " nodes" << std::endl;

    cfg.printMode();
    std::cout << "Log path: " << cfg.getLogPath() << std::endl;

    std::cout.flush();

    auto logC = std::make_shared<LogContainer>(static_cast<unsigned int>(cfg.getNodeCount()));
    auto tsq = std::make_shared<TSQueue>();

    auto lld = std::make_shared<LogLoader>(cfg.getLogPath(), cfg.getMode(), logC, tsq);
    bool fileOpened = lld->openLogFile();

    if (fileOpened) {
        lld->load();

        QApplication app(argc, argv);

        auto *mW = new MainWindow();
        mW->show();
        mW->setConfig(cfg, logC, lld, tsq);

        std::cout.flush();
        return QApplication::exec();
    }

    return 0;
}
