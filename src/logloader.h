#ifndef LOGLOADER_H
#define LOGLOADER_H

#include <string>
#include <configuration.h>
/*!
 * \brief The LogLoader is used to load data from the logfile
 */
class LogLoader
{
public:
    LogLoader();

    /*!
     * \brief load begins the loading of a logfile, it detaches a thread that reads it and terminates if mode is set to batch or stat, and keeps updating if RTime is selected
     * \param path path to the logfile
     * \param mode the selected mode
     */
    void load(std::string path,Configuration::MODE mode);
};

#endif // LOGLOADER_H
