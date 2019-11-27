#ifndef TSQUEUE_H
#define TSQUEUE_H
#include <queue>
#include <QWaitCondition>
#include <iostream>
#include <QMutex>
#include <QThread>
/*!
 * \brief The TSQueue class a ts queue implementation that can be broken to inhibit push
 * \author Francesco Franzini
 */
class TSQueue
{
private:
    std::queue<std::string*> queue;
    QMutex qLock;
    QWaitCondition cV;
    bool broken=false;
    bool batch=true;
public:

    /*!
     * \brief pop pops a string from the queue, sets valid to false if the queue was broken or empty. does not block if queue is empty and block is false
     * \param valid if the read string is meaningful
     * \param block set to true if the method has to block until an element is available or queue is broken
     * \return the read string
     */
    std::string * pop(bool &valid,bool block)
    {
        QMutexLocker lck(&qLock);
        if(block){
            while(queue.empty()){
                if(broken){
                    valid=false;
                    return nullptr;
                }
                cV.notify_one();
                cV.wait(&qLock);
            }
        }else if(queue.empty()){
            cV.notify_one();
            valid=false;
            return nullptr;
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
        QMutexLocker lck(&qLock);
        queue.push(el);
        cV.notify_all();
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

    /*!
     * \brief waitEmpty waits until the queue is fully emptied
     */
    void waitEmpty(){
        QMutexLocker lck(&qLock);
        while(!queue.empty() && !QThread::currentThread()->isInterruptionRequested()){
            cV.wait(&qLock);
        }
    }
};

#endif // TSQUEUE_H
