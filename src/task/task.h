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

        void operator()()
        {
            if (func)
               func();
        }
        std::function<void()> func;
    };

    template<typename R, typename... Arg>
    std::unique_ptr<Task> generateTask(const std::function<R(const Arg&...)>& f, const Arg&... arg)
    {
        if (f) {
            auto func = [f, arg...]() {
                f(arg...);
            };
            return std::make_unique<Task>(func);
        }
        return nullptr;
    }

    template<typename R, typename... Arg>
    std::unique_ptr<Task> generateTask(std::function<R(const Arg&...)> f, const Arg&... arg)
    {
        if (f) {
            auto func = [f, arg...]() {
                f(arg...);
            };
            return std::make_unique<Task>(func);
        }
        return nullptr;
    }

    template<typename F, typename... Arg>
    std::unique_ptr<Task> generateTask(F f, const Arg&... arg)
    {
        std::function<void(const Arg&...arg)> fun(f);
        if (fun) {
            auto func = [f, arg...]() {
                f(arg...);
            };
            return std::make_unique<Task>(func);
        }
        return nullptr;
    }

}
