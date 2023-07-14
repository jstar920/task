#include "taskListQueue.h"
#include "task.h"
#include "assure.h"
#include <thread>

using namespace task;
TEST_CASE("signgle_thread", "TaskListQueueTest")
{
    TaskListQueue queue;
    size_t count {0};
    std::thread t([&queue](){queue.run();});
    const auto TestFunc = [&count]()->void{
        ++count;
    };
    queue.addTask(std::move(generateTask(TestFunc)));
    queue.addTask(std::make_unique<Task>(TestFunc));
    queue.addTask(std::make_unique<Task>(TestFunc));
    queue.addTask(std::make_unique<Task>(TestFunc));
    queue.addTask(std::make_unique<Task>(TestFunc));

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(2s);
    queue.stop();
    t.join();
    ASSURE_EQ(count, 5);
}

size_t add2Return(size_t& count)
{
    count += 2;
    return count;
}

void add2(size_t& count)
{
    count += 2;
}

TEST_CASE("multi_thread", "TaskListQueueTest")
{
    TaskListQueue queue;
    size_t count {0};
    std::thread t([&queue](){queue.run();});
    const auto TestFunc = [&count](){
        ++count;
    };
    std::thread t1([&queue, TestFunc](){
        queue.addTask(std::move(generateTask(TestFunc)));
        queue.addTask(std::make_unique<Task>(TestFunc));
        queue.addTask(std::make_unique<Task>(TestFunc));
        queue.addTask(std::make_unique<Task>(TestFunc));
        queue.addTask(std::make_unique<Task>(TestFunc));
    });

    std::thread t2([&queue, &count](){
        for (size_t i = 0; i < 100; ++i)
            queue.addTask(std::move(generateTask(add2Return, count)));
    });

    std::thread t3([&queue, TestFunc](){
        for (size_t i = 0; i < 100; ++i)
            queue.addTask(std::make_unique<Task>(TestFunc));
    });

    std::thread t4([&queue, TestFunc](){
        for (size_t i = 0; i < 100; ++i)
            queue.addTask(std::make_unique<Task>(TestFunc));
    });

    std::thread t5([&queue, TestFunc](){
        for (size_t i = 0; i < 100; ++i)
            queue.addTask(std::make_unique<Task>(TestFunc));
    });
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(2s);
    queue.stop();
    t.join();
    ASSURE_EQ(count, 405);
}
