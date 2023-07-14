#include "taskListQueue.h"
#include "task.h"

#include <chrono>

namespace task
{
   void TaskListQueue::addTask(TaskPtr t)
   {
       std::lock_guard<std::mutex> guard(mutex_);
       tasks_.emplace_back(std::move(t));
       cv_.notify_one();
   }

   void TaskListQueue::run()
   {
       if (state_.load() != State::Initial)
       {
           abort();
       }

       state_.store(State::Running);
       while (state_.load() != State::Stopped)
       {
           if (queueEmpty())
           {
               std::unique_lock<std::mutex> lk(mutex_);
               cv_.wait(lk, [this](){return (!tasks_.empty() || state_.load() == State::Stopped);});
               if (state_.load() == State::Stopped)
               {
                   break;
               }
           }

           auto t = std::move(tasks_.front());
           {
               std::lock_guard<std::mutex> guard(mutex_);
               tasks_.pop_front();
           }
           (*t)();
       }
   }

   void TaskListQueue::stop()
   {
       state_.store(State::Stopped);
       cv_.notify_one();
   }

   bool TaskListQueue::queueEmpty() const
   {
       std::lock_guard<std::mutex> guard(mutex_);
       return tasks_.empty();
   }
}