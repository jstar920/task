#include <memory>
#include <list>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace task
{
    using TaskPtr = std::unique_ptr<struct Task>;
    class TaskListQueue
    {
    public:
        void addTask(TaskPtr t);
        void run();
        void stop();
    private:
        bool queueEmpty() const;
    private:
        std::list<TaskPtr> tasks_;
        mutable std::mutex mutex_;
        enum class State
        {
            Initial,
            Running,
            Stopped
        };
        std::atomic<State> state_ {State::Initial};
        std::condition_variable cv_;
    };
}