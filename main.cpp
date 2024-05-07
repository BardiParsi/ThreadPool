#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <condition_variable>
#include <cassert>
#include <vector>
#include "lib/QueueList.h"
#include "lib/LOGGER.h"
#include <random>
#include <future>

/*
This project requires C++20 or above.
Minimum threads requirement for your device to run this program is 6.
Asserts are for debugging purposes only and should be removed for the release version.
This is a Thread Pool pattern used for concurrency in handling multiple heavy tasks by dividing the CPU threads
into smaller portions and executing the heavy tasks concurrently.
*/

static const size_t CPU_THREADS = std::thread::hardware_concurrency(); //number of threads in the device's CPU}
static const size_t NUMBER_OF_TASKS = 3; //The number of heavy tasks that you want to implement concurrently
static const size_t TASK_SIZE = (CPU_THREADS > NUMBER_OF_TASKS) ? (CPU_THREADS / NUMBER_OF_TASKS) - 1 : 1;//nums of dedicated thread/task
//TASK_SIZE is the number of threads that we dedicate to process each heavyTask() function
//If the the number of threads inside a device is not greater than 3; the each task will be dedicated only 1 thread at the time.

using std::cout;
using std::endl;

uint16_t generateRandomNum() {
    static std::mt19937_64 mtRand(std::random_device{}());
    std::uniform_int_distribution<int> randDist(1, 3);
    return randDist(mtRand);
}

[[nodiscard]] std::shared_future<uint16_t> futureGenerator() {
    return std::async(std::launch::async, [](){ return generateRandomNum();});
}

//This is a heavy task simulator function. to simulate random heavy task.
void heavyTask1(QueueList<std::shared_future<uint16_t>>& queue, const size_t& TASK_SIZE) {
    std::vector<std::jthread> pushVec;
    //we use random delay time to make it look more realistic.
    auto delay = generateRandomNum();
    std::this_thread::sleep_for(std::chrono::seconds(delay));
    //simulated heavy task using the device processor
    for (size_t i = 0; i < TASK_SIZE; i++) {
        pushVec.emplace_back([&](){ queue.push(futureGenerator()); queue.pop(); });
    }
    for (auto& t : pushVec) {
        t.join();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(40));
    LOGGER::INFO("The Task is completed\n");
}

//The second heavy task
void heavyTask2(QueueList<std::shared_future<uint16_t>>& queue, const size_t& TASK_SIZE) {
    std::vector<std::jthread> pushVec;
    auto delay = generateRandomNum();
    std::this_thread::sleep_for(std::chrono::seconds(delay));
    for (size_t i = 0; i < TASK_SIZE; i++) {
        pushVec.emplace_back([&](){ queue.push(futureGenerator()); queue.pop(); });
    }
    for (auto& t : pushVec) {
        t.join();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(40));
    LOGGER::INFO("The Task is completed\n");
}

//The third heavy task
void heavyTask3(QueueList<std::shared_future<uint16_t>>& queue, const size_t& TASK_SIZE) {
    std::vector<std::jthread> pushVec;
    auto delay = generateRandomNum();
    std::this_thread::sleep_for(std::chrono::seconds(delay));
    for (size_t i = 0; i < TASK_SIZE; i++) {
        pushVec.emplace_back([&](){ queue.push(futureGenerator()); queue.pop(); });
    }
    for (auto& t : pushVec) {
        t.join();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(40));
    LOGGER::INFO("The Task is completed\n");
}

int main() {

    std::vector<std::jthread> vector;
    QueueList<std::shared_future<uint16_t>> queue(NUMBER_OF_TASKS);
    try{
        vector.emplace_back([&](){ heavyTask1(queue, TASK_SIZE); });
        vector.emplace_back([&](){ heavyTask2(queue, TASK_SIZE); });
        vector.emplace_back([&](){ heavyTask3(queue, TASK_SIZE); });

        for (auto& v : vector) {
          v.join();
        }
    } catch(const std::exception& e) {
        LOGGER::INFO("Exception caught: " + std::string(e.what()) + "\n");
    }

    system("pause");

    return 0;
}
