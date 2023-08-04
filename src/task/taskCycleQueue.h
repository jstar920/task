#include <memory>
#include <atomic>
#include <thread>

namespace task
{
    using TaskPtr = std::unique_ptr<struct Task>;

    template<size_t N>
    class TaskCycleQueue
    {
        static_assert(N > 1, "queue size should >= 2");
        enum Type
        {
            Invalid,
            Writing,
            Valid
        };
        struct Element
        {
             std::atomic<Type> status { Type::Invalid };
             TaskPtr task;
        };
        Element* elements_;
        std::atomic<size_t> push_index_ {0};
        std::atomic<size_t> size_ {0};

        std::atomic<bool> stop_ {false};
        
        void init()
        {
            elements_ = new Element[N];
        }

    public:
        TaskCycleQueue()
        {
            init();
        }

        size_t size()
        {
            return size_.load();
        }

        void push(TaskPtr task)
        {
            Type expected {Type::Invalid};
            while (!elements_[push_index_].status.compare_exchange_weak(expected, Type::Writing))
            {
                push_index_ = (push_index_ + 1) % N;
            }

            elements_[push_index_].task = std::move(task);
            elements_[push_index_].status = Type::Valid;
            ++size_;
        }

        void stop()
        {
            stop_.store(true);
        }

        void process()
        {
            if (stop_ == false)
            {
                return;
            }

            size_t pop_index {0};
            while (!stop_)
            {
                Type expected {Type::Valid};
                
                while (!(elements_[pop_index].status != Type::Valid))
                {
                    if (size == 0 && pop_index == push_index_)
                    {
                        using namespace std::chrono_literals;
                        std::this_thread::sleep_for(50ms);
                        continue;
                    }
                    pop_index = (pop_index + 1) % N;
                }

                auto task = std::move(elements_[pop_index].task);
                elements_[pop_index].status = Type::Invalid;
                --size;
                task->execute();
            }
        }
    };
}