//----------- TimerExample.cpp
#include "rxcpp/rx.hpp"
#include "rxcpp/rx-test.hpp"
#include <iostream>
#include <thread>
#include <chrono>
int main()
{
    auto scheduler = rxcpp::observe_on_new_thread();

    auto period = std::chrono::seconds(3);

    auto values = rxcpp::observable<>::timer(period, scheduler).finally([]()
                                                                        { std::cout << "The final action, thread id: " << std::this_thread::get_id() << std::endl; });
    values.as_blocking().subscribe(
        [](int v)
        { std::cout << "OnNext: " << v << "thread id: " << std::this_thread::get_id() << std::endl; },

        []()
        { std::cout << "OnCompleted, thread id: " << std::this_thread::get_id() << std::endl; });
    std::cout << "main thread id: " << std::this_thread::get_id() << std::endl;
    //必须在主线程sleep，否则finally中的内容打印不全，因为程序退出后，线程销毁
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}