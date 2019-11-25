#ifndef LOGLOADER_H
#define LOGLOADER_H
#include "logcontainer.h"
#include "tsqueue.h"


#include <string>
#include <configuration.h>
#include <thread>
/*!
 * \brief The LogLoader is used to load data from the logfile
 * \author Francesco Franzini
 */
class LogLoader
{
public:
    /*!
     * \brief load begins the loading of a logfile, it detaches a thread that reads it and terminates if mode is set to batch or stat, and keeps updating if RTime is selected
     * \param path path to the logfile
     * \param mode the selected mode
     * \param logC the log container object
     * \param queue the queue where to push the text lines
     */
    void load(const std::string& path, Configuration::MODE mode, const std::shared_ptr<LogContainer>& logC, const std::shared_ptr<TSQueue>& queue);

    /*!
     * \brief stop stop stops the reading thread(if not already terminated) at the next iteration
     */
    void stop();
private:
    bool terminate=false;
    static void loadBatch(const std::string& path,const std::shared_ptr<LogContainer>&logC,const std::shared_ptr<TSQueue>& queue);
    static void loadStat(const std::string& path,const std::shared_ptr<LogContainer>&logC,const std::shared_ptr<TSQueue>& queue);
    static void loadRT(const std::string& path,const std::shared_ptr<LogContainer>&logC,const std::shared_ptr<TSQueue>& queue);
    std::unique_ptr<std::thread> loaderThread=nullptr;
    std::shared_ptr<TSQueue> queue=nullptr;
};

#endif // LOGLOADER_H
