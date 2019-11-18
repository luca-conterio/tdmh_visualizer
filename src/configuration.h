#ifndef CONFIGURATION_H
#define CONFIGURATION_H


#include <string>
/*!
 * \brief The Configuration class, used to store configuration variables
 */
class Configuration
{
public:

    /*!
     * \brief The MODE enum represents the three supported ways to read a logfile
     */
    enum MODE {BATCH, STAT, RTIME};
private:

    int node_count=default_nodes;
    std::string log_path="log.txt";
    MODE mode=BATCH;
public:
    /*!
     * \brief max_nodes default number of nodes
     */
    static const int default_nodes=16;

     Configuration();

     /*!
      * \brief loadCfg loads configuration from file, if ptr is null or parameters are missing, defaults are used
      * \param ptr path to config file
      */
     void loadCfg(char * ptr);

     /*!
      * \brief getNodeCount node_count getter
      * \return the node_count
      */
     int getNodeCount() const;

     /*!
      * \brief getLogPath log_path getter
      * \return log path string
      */
     std::string getLogPath() const;

     /*!
      * \brief getMode returns the mode that has been selected
      * \return the selected mode
      */
     MODE getMode();
};

#endif // CONFIGURATION_H
