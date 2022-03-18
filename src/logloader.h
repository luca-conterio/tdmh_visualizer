#ifndef LOGLOADER_H
#define LOGLOADER_H

#include "logcontainer.h"
#include "tsqueue.h"
#include <string>
#include <fstream>
#include <configuration.h>
#include <QThread>

/*!
 * \brief The LogLoader is used to load data from the logfile
 * \author Francesco Franzini
 */
class LogLoader : public QThread
{
    Q_OBJECT

public:

    /*!
     * \brief Initializes and sets parameters
     * \param path path to the logfile
     * \param mode the selected mode
     * \param logC the log container object
     * \param queue the queue where to push the text lines
     */
    LogLoader(const std::string& path, Configuration::MODE mode, const std::shared_ptr<LogContainer> &logC, const std::shared_ptr<TSQueue>& queue);

    /*!
     * \brief load begins the loading of a logfile, it detaches a thread that reads it and terminates if mode is set to batch or stat, and keeps updating if RTime is selected
     */
    void load();

    bool openLogFile();

    /*!
     * \brief stop stops the reading thread (if not already terminated) at the next iteration
     */
    void stop();
private:
    const std::string path;
    const Configuration::MODE mode;
    const std::shared_ptr<LogContainer> logC;
    const std::shared_ptr<TSQueue> queue;

    bool terminate = false;
    bool logError = false;
    std::ifstream file;

    void loadBatch(const std::shared_ptr<LogContainer>&logC, const std::shared_ptr<TSQueue>& queue, const bool callRTAfter);
    void loadStat(const std::shared_ptr<LogContainer>&logC, const std::shared_ptr<TSQueue>& queue);
    void loadRT(unsigned int lineN, std::ifstream &file, const std::shared_ptr<LogContainer>&logC, const std::shared_ptr<TSQueue>& queue);
    const int realTimeSleep = 1000;
    // QThread interface
protected:
    /*!
     * \brief run starts the loading thread with the given configuration
     */
    void run() override;
};

#endif // LOGLOADER_H
