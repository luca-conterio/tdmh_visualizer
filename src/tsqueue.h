#ifndef TSQUEUE_H
#define TSQUEUE_H
#include <queue>
#include <mutex>
#include <condition_variable>
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
    std::string pop()
    {
        std::unique_lock<std::mutex> lck(qLock);

        while(queue.empty()){
            if(broken){return "X";}
            cV.wait(lck);
        }
        std::string el=queue.front();
        queue.pop();
        return el;
    }
    void push(std::string el)
    {
        if(broken)return;
        std::unique_lock<std::mutex> lck(qLock);
        queue.push(el);
        cV.notify_one();
    }

    bool isBroken(){
        return this->broken;
    }

    void breakQueue(){
        this->broken=true;
    }
};

#endif // TSQUEUE_H
