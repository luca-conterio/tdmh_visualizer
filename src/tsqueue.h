#ifndef TSQUEUE_H
#define TSQUEUE_H
#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>
/*!
 * \brief The TSQueue class a ts queue implementation that can be broken to inhibit push
 * \author Francesco Franzini
 */
class TSQueue
{
private:
    std::queue<std::string*> queue;
    std::mutex qLock;
    std::condition_variable cV;
    bool broken=false;
    bool batch=true;
public:

    /*!
     * \brief pop pops a string from the queue, sets valid to false if the queue was broken or empty. does not block if queue is empty
     * \param valid if the read string is meaningful
     * \return the read string
     */
    std::string * pop(bool &valid)
    {
        std::unique_lock<std::mutex> lck(qLock);

        /*while(queue.empty()){
            if(broken){
                valid=false;
                return "";
            }
            cV.wait(lck);
        }*/
        if(queue.empty()){
            valid=false;
            return new std::string("");
        }
        std::string* el=queue.front();
        queue.pop();
        valid=true;
        return el;
    }

    /*!
     * \brief push if the queue is not broken pushes a string into the queue and notifies a waiting thread
     * \param el the string to be pushed
     */
    void push(std::string* el)
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

    /*!
     * \brief toRT switches the batch flag off to signal that big batches of lines will not be loaded any more
     */
    void toRT(){
        this->batch=false;
    }

    /*!
     * \brief isBatch checks if the batch flag is set
     * \return  the batch flag
     */
    bool isBatch(){
        return this->batch;
    }
};

#endif // TSQUEUE_H
