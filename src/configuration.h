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
    std::string img_path="image.jpg";
    MODE mode=BATCH;

    /*!
     * \brief pushNode pushes a new node on the nodelist
     * \param i index
     * \param x x position
     * \param y y position
     */
    void pushNode(int i,int x, int y);

    void processTuple(const std::string& t);

    static inline void trim(std::string &s);
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
     MODE getMode() const;

     /*!
      * \brief getImgPath returns the image file path
      * \return image log path
      */
     std::string getImgPath() const;
};

#endif // CONFIGURATION_H
