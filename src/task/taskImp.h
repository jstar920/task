#include <memory>
#include <list>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace task
{
    using TaskPtr = std::unique_ptr<struct Task>;
    class TaskManager
    {
    public:
        void addTask(TaskPtr t);
        void run();
        void stop();
    private:
        std::list<TaskPtr> tasks_;
        std::mutex mutex_;
        std::atomic_bool stop_ {true};
        std::condition_variable cv_;
    };
}