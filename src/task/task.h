#include "apply.hpp"
#include <functional>
#include <memory>
namespace task
{
    struct Task
    {
        Task() = delete;
        Task(const std::function<void()>& func)
            : func(func)
        {
        }

        template<typename F, typename R, typename... Args>
        Task(F f, Args&&...args)
        {
            std::function<R(Args...args)> fun(f);
            func = [fun, args = std::forward_as_tuple(args...)](){
                cpputils::apply(fun, args); 
            };
        }

        void operator()()
        {
            if (func)
               func();
        }
        std::function<void()> func;
    };

    template<typename F, typename... Args>
    std::unique_ptr<Task> generateTask(F f, Args... args)
    {
        auto func = [f, args = std::forward_as_tuple(args...)]() {
            cpputils::apply(f, args);
        };
        return std::make_unique<Task>(func);
    }
}
