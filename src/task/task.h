#include <functional>
struct Task
{
    Task() = delete;
    template<typename R, typename... Arg>
    Task(const std::function<R(const Arg&...)>& f, const Arg&... arg)
    {
        if (f) {
            func = [f, arg...]() {
                f(arg...);
            };
        }
    }

    void operator()()
    {
        if (func)
           func();
    }
    std::function<void()> func;
};