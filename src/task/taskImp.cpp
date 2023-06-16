#include "taskImp.h"

namespace task
{
   void TaskManager::addTask(TaskPtr t)
   {
       if (!stop_)
       {
           std::lock_guard<std::mutex> guard(mutex_);
           tasks_.emplace_back(std::move(t));
       }
   }

   void TaskManager::run()
   {
   }

   void TaskManager::stop()
   {
       stop_ = true;
   }
}