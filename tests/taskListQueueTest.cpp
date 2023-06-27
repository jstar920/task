#include "taskListQueue.h"
#include "task.h"
#include "assure.h"

using namespace task;
TEST_CASE("signgle_thread", "TaskListQueueTest")
{
    TaskListQueue queue;
    size_t count {0};
    const auto TestFunc = [&count](){
        ++count;
    };
    queue.addTask(std::move(generateTask(TestFunc)));
//    queue.addTask(std::make_unique<Task>(TestFunc));
//    queue.addTask(std::make_unique<Task>(TestFunc));
//    queue.addTask(std::make_unique<Task>(TestFunc));

    queue.run();
    queue.stop();
    ASSURE_EQ(count, 5);
}

