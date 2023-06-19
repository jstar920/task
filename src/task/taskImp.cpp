#include "taskImp.h"
#include "task.h"

#include <chrono>

namespace task
{
   void TaskQueue::addTask(TaskPtr t)
   {
       std::lock_guard<std::mutex> guard(mutex_);
       tasks_.emplace_back(std::move(t));
   }

   void TaskQueue::run()
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

   void TaskQueue::stop()
   {
       stop_ = true;
   }

   bool TaskQueue::queueEmpty() const
   {
       std::lock_guard<std::mutex> guard(mutex_);
       return tasks_.empty();
   }
}