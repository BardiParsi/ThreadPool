#pragma once
#include "LOGGER.h"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <cassert>
#include <exception>

template<typename E>
class QueueList
{
private:
    std::queue<E> queue;
    std::mutex mtx;
    std::condition_variable condVar;
    size_t maxSize{}; //the maximum size of queue is for a better performance efficiency depended on each device
public:
    QueueList(const size_t& maxSize) : maxSize(maxSize) {
        assert(maxSize > 0); //size of the queue should be bigger than 0 or this function has no reason to be implemented
    }

    void push(const E& elem) {
        std::unique_lock<std::mutex> lock(mtx);
        condVar.wait(lock, [&](){ return queue.size() < maxSize; });
        if (queue.size() < maxSize) {
            queue.push(elem);
        }
        lock.unlock();
        condVar.notify_one();
        LOGGER::INFO("The element is added to the Queue.\n");
    }

    void pop() {
        std::unique_lock<std::mutex> lock(mtx);
        condVar.wait(lock, [&](){ return !queue.empty(); });
        if (!queue.empty()) {
            auto elem = queue.front();
            uint16_t value = elem.get();
            queue.pop();
            lock.unlock();
            condVar.notify_one();
            LOGGER::INFO("The element has been deleted. The Value is: " + std::to_string(value) + " \n");
        }
        else {
            throw std::runtime_error("The queue is empty!\n");
        }
    }

    ~QueueList() = default;

};
