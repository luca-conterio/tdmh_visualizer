#ifndef LOGLINE_H
#define LOGLINE_H
#include <boost/dynamic_bitset.hpp>
/*!
 * \brief The LogLine class represents an entry in the log that carries adjacency values
 */
class LogLine
{
public:
    /*!
     * \brief nodeId id of the node the masks refer to
     */
    const unsigned int nodeId;

    /*!
     * \brief lineN line number
     */
    const unsigned int lineN;

    /*!
     * \brief strongMask a bitvector containing the mask of strong links
     */
    const boost::dynamic_bitset<> strongMask;

    /*!
     * \brief weakMask a bitvector containing the mask of weak links
     */
    const boost::dynamic_bitset<> weakMask;

    /*!
     * \brief LogLine sets all the values of the log line
     * \param nodeId id of the node the masks refer to
     * \param lineN line number
     * \param strongMask a bitvector containing the mask of strong links
     * \param weakMask a bitvector containing the mask of weak links
     */
    LogLine(unsigned int nodeId, unsigned int lineN, boost::dynamic_bitset<> strongMask, boost::dynamic_bitset<> weakMask);

    /*!
     * \brief comparator implements less than operator for line number upper_bound search
     * \param val value to compare to
     * \param a LogLine to decide
     * \return  val<a.lineN
     */
    static bool comparator(unsigned int val,const LogLine& a){
      return val<a.lineN;
    }
};

#endif // LOGLINE_H
