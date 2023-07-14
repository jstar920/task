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
#if __cplusplus >= 202002L
    template<typename R, typename... Args>
    std::unique_ptr<Task> generateTask(std::function<R(Args...)> f, Args... args)
    {
        if (f) {
            auto func = [f, ...args = std::forward(args)]() {
                f(std::forward(args)...);
            };
            return std::make_unique<Task>(func);
        }
        return nullptr;
    }
#elif __cplusplus >= 201703L && __cplusplus < 202002L
    template<typename R, typename... Args>
    std::unique_ptr<Task> generateTask(std::function<R(Args...)> f, Args... args)
    {
        if (f) {
            auto func = [f, args = std::forward_as_tuple(args...)]() {
                std::apply(f, std::forward(args));
            };
            return std::make_unique<Task>(func);
        }
        return nullptr;
    }
#elif __cplusplus >= 201402L && __cplusplus < 201703L
    template<typename R, typename... Args>
    std::unique_ptr<Task> generateTask(std::function<R(Args...)> f, Args... args)
    {
        if (f) {
            auto func = [f, args = std::forward_as_tuple(args...)]() {
                std::apply(f, std::forward(args));
            };
            return std::make_unique<Task>(func);
        }
        return nullptr;
    }
#elif __cplusplus >= 201103L && __cplusplus < 201402L
#endif
}
