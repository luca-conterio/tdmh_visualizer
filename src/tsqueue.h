#ifndef TSQUEUE_H
#define TSQUEUE_H
#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>
/*!
 * \brief The TSQueue class a ts queue implementation
 */
class TSQueue
{
private:
    std::queue<std::string> queue;
    std::mutex qLock;
    std::condition_variable cV;
    bool broken=false;
public:

    /*!
     * \brief pop pops a string from the queue, sets valid to false if the queue was broken and empty. blocks if queue is empty
     * \param valid if the read string is meaningful
     * \return the read string
     */
    std::string pop(bool &valid)
    {
        std::unique_lock<std::mutex> lck(qLock);

        while(queue.empty()){
            if(broken){
                valid=false;
                return "";
            }
            cV.wait(lck);
        }
        std::string el=queue.front();
        queue.pop();
        valid=true;
        return el;
    }

    /*!
     * \brief push if the queue is not broken pushes a string into the queue and notifies a waiting thread
     * \param el the string to be pushed
     */
    void push(const std::string& el)
    {
        if(broken)return;
        std::unique_lock<std::mutex> lck(qLock);
        queue.push(el);
        cV.notify_one();
    }

    /*!
     * \brief isBroken checks if the queue is broken
     * \return broken
     */
    bool isBroken(){
        return this->broken;
    }

    /*!
     * \brief breakQueue breaks the queue, preventing new strings from being added
     */
    void breakQueue(){
        this->broken=true;
    }
};

#endif // TSQUEUE_H
