#ifndef CONFIGURATION_H
#define CONFIGURATION_H


#include <string>
#include <vector>
/*!
 * \brief The Configuration class, used to store configuration variables
 * \author Francesco Franzini
 */
class Configuration
{
public:

    /*!
     * \brief The MODE enum represents the three supported ways to read a logfile
     */
    enum MODE {BATCH, STAT, RTIME};
private:

    //Default values used if not specified in the configuration file
    int node_count=default_nodes;
    std::string log_path="log.txt";
    bool log_path_set = false;
    std::string img_path="image.jpg";
    MODE mode=BATCH;
    bool batchFirst=false;

    std::vector<std::pair<int,int>> nodeList;

    /*!
     * \brief pushNode pushes a new node on the nodelist
     * \param i index
     * \param x x position
     * \param y y position
     */
    void pushNode(size_t i, int x, int y);


    /*!
     * \brief processTuple processes a tuple and if valid pushes it to the node list
     * \param t tuple to process
     */
    void processTuple(const std::string& t);

    /*!
     * \brief trim trims both ends of a string
     * \param s string to be trimmed
     */
    static inline void trim(std::string &s);

public:

    /*!
     * \brief default_nodes default number of nodes to be used
     */
    static const int default_nodes=16;

     /*!
      * \brief loadCfg loads configuration from file, if ptr is null or parameters are missing, defaults are used
      * \param ptr path to config file
      */
     void loadCfg(const char *ptr);

     void setLogFilePath(const char *ptr);

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
     MODE getMode() const;

     /*!
      * \brief getImgPath returns the image file path
      * \return image log path
      */
     std::string getImgPath() const;

     /*!
      * \brief getNodeList nodelist getter
      * \return the list of pairs (x,y) of each note of index i
      */
     std::vector<std::pair<int, int> > getNodeList() const;

     /*!
      * \brief getBatchFirst returns true if the batchFirst flag is set
      * \return batchFirst flag
      */
     bool getBatchFirst() const;
};

#endif // CONFIGURATION_H
