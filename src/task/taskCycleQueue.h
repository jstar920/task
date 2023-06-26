#include <memory>
#include <array>
#include <mutex>
#include <condition_variable>
#include <atomic>


namespace task
{
    using TaskPtr = std::unique_ptr<struct Task>;
    template<size_t N>
    class TaskCycleQueue
    {
        static_assert(N > 1);
    public:
        TaskCycleQueue()
        {
        }
        
        void addTask(TaskPtr t)
        {
            auto nextAvailable = nextAvailable();
            tasks_[nextAvailable.load()] = std::move(t);
        }

        void run()
        {
            while (!stop_) {
                 auto next = nextExecute();
            }
        }

        void stop()
        {
            stop_ = true;
        }
    private:
        bool queueEmpty() const;
        size_t nextAvailable()
        {
            using namespace std::chrono_literals;
            std::unique_lock<std::mutex> lk(mutex_);
            cv_.wait_for(lk, 10ms, [this](){return !full_;});
            auto nextAvailable = nextAvailable_;
            nextAvailable_ = ++nextAvailable_ % N;
            if (nextAvailable_ == end_)
            {
                full_ = true;
            }
            return nextAvailable;
        }

        size_t nextExecute()
        {
            using namespace std::chrono_literals;
            std::unique_lock<std::mutex> lk(mutex_);
            cv_.wait_for(lk, 10ms, [this](){return (end_ != nextAvailable_ || full_);});
            auto nextExe = end_;
            end_ = ++end_ % N;
            full_ = false;
            return nextExe;
        }
    private:
        std::atomic<TaskPtr, N>> tasks_;
        size_t nextAvailable_{ 0 };
        size_t end_{ 0 };
        bool full_ { false };
        mutable std::mutex mutex_;
        std::atomic_bool stop_ {true};
        std::condition_variable cv_;
    };
}