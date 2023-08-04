#include "taskListQueue.h"
#include "task.h"
#include "assure.h"
#include <thread>

using namespace task;
namespace TaskListTest
{

class Operator
{
public:
    int count1 {0};
    void operator()() {
        count1++;
    };

    int count2 {0};
    void operator()(int a, const float& b)
    {
        count2++;
    }

    int count3 {0};
    void operator()(int& a, const float& b, float c)
    {
        count3++;
        a = count3;
    }
};

void overloadFun(int a)
{
}
/*
void overloadFun(int a, float b)
{
}
*/

class TestC
{
public:
    void f(int a, int b, const std::string& c)
    {
    }
};

TEST_CASE("signgle_thread", "TaskListQueueTest")
{
    TaskListQueue queue;
    size_t count {0};
    std::thread t([&queue](){queue.run();});
    const auto TestFunc = [&count]()->void{
        ++count;
    };
    queue.addTask(std::move(generateTask(TestFunc)));
    Operator op;
    int a;
    float b;
    queue.addTask(std::make_unique<Task>(op));
    //queue.addTask(std::make_unique<Task>(op, 1, 1.0));
    queue.addTask(std::move(generateTask(op, (int)1, (float)1.0)));
    queue.addTask(std::move(generateTask(op, (int)1, (float)1.0)));
    queue.addTask(std::move(generateTask(op, a, b, 1.9f)));
    queue.addTask(std::make_unique<Task>(TestFunc));

    TestC tc;
    queue.addTask(std::move(generateTask(&TestC::f, tc, 2, 1, "")));
    queue.addTask(std::move(generateTask(&TestC::f, &tc, 2, 1, "")));
//    auto pTc = std::make_shared<TestC>();
//    queue.addTask(std::move(generateTask(&TestC::f, pTc, 2, 1, "")));

    //queue.addTask(std::make_unique<Task>(new Task(overloadFun, 1)));

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

void add2(size_t& count, int n)
{
    count = count + n + 2;
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

}
