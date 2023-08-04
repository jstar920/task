#include "taskCycleQueue.h"
#include "task.h"
#include "assure.h"
#include <thread>

using namespace task;
namespace TaskCycleQueueTest
{

TEST_CASE("signgle_thread", "TaskCycleQueueTest")
{
    TaskCycleQueue<1000> queue;
    
    const auto f = [](int& i){
        i++;
    };
    const auto addTask = [&queue, f](){
        static int i;
        queue.push(std::move(generateTask(f, i)));
    };

    std::thread t[100];
    for (size_t i = 0; i < 100; ++i)
    {
        t[i] = std::thread([addTask](){
            for (size_t j = 0; j < 10; ++j)
                addTask();
        });
    }
    for (size_t i = 0; i < 100; ++i)
    {
        t[i].join();
    }

    ASSURE_EQ(1000, queue.size());
}

}

