#ifndef LOGCONTAINER_H
#define LOGCONTAINER_H
#include "logline.h"

#include <vector>
#include <boost/dynamic_bitset.hpp>
/*!
 * \brief The LogContainer class contains the information extracted from the log file so far
 */
class LogContainer
{
private:
    std::vector<std::vector<LogLine>*> store;
    boost::dynamic_bitset<> toDynSet(std::vector<char> mask) const;

public:

    /*!
     * \brief LogContainer initializes the structure
     * \param initSize initial number of nodes
     */
    LogContainer(int );

    /*!
     * \brief addLine adds a line to the store
     * \param nodeId id of the node the line refers to
     * \param lineN line number
     * \param strong strong links mask
     * \param weak weak links mask
     */
    void addLine(unsigned int nodeId, unsigned int lineN, std::vector<char> strong , std::vector<char> weak);

    /*!
     * \brief findLine returns the last line before maxLine(inclusive) referring to a particular node
     * \param nodeId the node to fetch
     * \param maxLine maximum line number to consider
     * \return the LogLine, if none is present a zero-masks one is returned
     */
    LogLine findLine(unsigned int nodeId, unsigned int maxLine) const;

    ~LogContainer();
};

#endif // LOGCONTAINER_H
