#include "taskListQueue.h"
#include "task.h"

#include <chrono>

namespace task
{
   void TaskListQueue::addTask(TaskPtr t)
   {
       std::lock_guard<std::mutex> guard(mutex_);
       tasks_.emplace_back(std::move(t));
   }

   void TaskListQueue::run()
   {
       while (!stop_)
       {
           if (queueEmpty())
           {
               std::unique_lock<std::mutex> lk(mutex_);
               using namespace std::chrono_literals;
               cv_.wait_for(lk, 100ms, [this](){return tasks_.empty();});
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
       stop_ = true;
   }

   bool TaskListQueue::queueEmpty() const
   {
       std::lock_guard<std::mutex> guard(mutex_);
       return tasks_.empty();
   }
}