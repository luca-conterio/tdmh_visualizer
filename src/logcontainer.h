#ifndef LOGCONTAINER_H
#define LOGCONTAINER_H
#include "logline.h"
#include <vector>
#include <mutex>
#include <memory>
#include <climits>
#include <set>
/*!
 * \brief The LogContainer class contains the information extracted from the log file so far
 * \author Francesco Franzini
 */
class LogContainer
{
private:

    //Topology
    const std::string prefix="[U] Topo ";
    std::mutex storeMutex;
    std::vector<std::unique_ptr<std::vector<LogLine>>> store;
    /*!
     * \brief toBoolVec transforms a vector of chars in a vector of bools, characters different from '0' and '1' are translated to 0
     * \param mask input mask as a vector of chars
     * \return  pointer to the mask as a vector of bools
     */
    std::unique_ptr<std::vector<bool>> toBoolVec(const std::vector<char> &mask) const;

    void updateMatSize(unsigned int newCount);

    //Stat
    unsigned int tempThresh=0;
    unsigned int maxNode=0;
    std::vector<std::vector<unsigned long>> timedMat;
    std::vector<std::vector<unsigned long>> untimedMat;

    std::vector<std::vector<double>> timedPercMat;
    std::vector<std::vector<double>> untimedPercMat;

    unsigned long firstTimestamp= ULONG_MAX;
    unsigned long lastTimestamp=0;
    unsigned long currentTimestamp=0;
    std::set<std::pair<unsigned int, unsigned int>> currentLinks;

    unsigned int emittedTopologies=0;
public:

    /*!
     * \brief LogContainer initializes the structure
     * \param initSize initial number of nodes
     */
    LogContainer(unsigned int );

    /*LogContainer(const LogContainer &oth)=delete ;
    LogContainer &operator =(const LogContainer&) = delete;
    LogContainer(const LogContainer &&oth)=delete;
    LogContainer &operator =(const LogContainer&&) = delete;*/
    /*!
     * \brief addLine adds a line to the store
     * \param nodeId id of the node the line refers to
     * \param lineN line number
     * \param strong strong links mask
     * \param weak weak links mask
     */
    void addLine(unsigned int nodeId, unsigned int lineN, const std::vector<char> &strong , const std::vector<char> &weak);

    /*!
     * \brief findLine returns the last line before maxLine(inclusive) referring to a particular node
     * \param nodeId the node to fetch
     * \param maxLine maximum line number to consider
     * \return the LogLine, if none is present a zero-masks one is returned
     */
    LogLine findLine(unsigned int nodeId, unsigned int maxLine);

    /*!
     * \brief getSize returns the current size of the store(in number of nodes present)
     * \return the number of seen nodes
     */
    unsigned int getSize();

    /*!
     * \brief process processes a line and if useful updates the store
     * \param lineN line number
     * \param line line to be processed
     */
    void process(unsigned int lineN, const std::string &line);

    /*!
     * \brief processStat processes a line in statistic mode
     * \param line line to be processed
     */
    void processStat(const std::string &line);

    /*!
     * \brief getTimedAvail returns the availability matrix
     * \param time set to true to get timed availabilities
     */
    std::vector<std::vector<double>> getAvail(bool time);

    /*!
     * \brief getTempThresh returns the line threshold beyond which the non temporal graph has to be displayed
     * \return the tempThresh value
     */
    unsigned int getTempThresh() const;

    /*!
     * \brief getTempThresh sets the line threshold beyond which the non temporal graph has to be displayed
     * \param value the tempThresh value
     */
    void setTempThresh(unsigned int value);

    /*!
     * \brief lastTimed signals that all the lines have been processed, used in stat mode
     */
    void lastLine();
};

#endif // LOGCONTAINER_H
