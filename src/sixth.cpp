//------------------ sixth.cpp
#include "rxcpp/rx.hpp"
#include <iostream>
#include <array>
int main()
{
    auto values = rxcpp::observable<>::just(1);
    values.subscribe(
        [](int v)
        { printf("OnNext: %d\n", v); },
        []()
        { printf("OnCompleted\n"); });
}