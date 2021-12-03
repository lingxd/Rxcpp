//------------------ Map_With_Pipe.cpp
#include "rxcpp/rx.hpp"
namespace Rx
{
    using namespace rxcpp;
    using namespace rxcpp::sources;
    using namespace rxcpp::operators;
    using namespace rxcpp::util;
}
using namespace Rx;
#include <iostream>
int main()
{
    auto ints = rxcpp::observable<>::range(1, 10) |
                map([](int n)
                    { return n * n; });

    auto ints2 = rxcpp::observable<>::range(1, 10);
    auto intsFromMap = ints2.map([](int n)
                                 { return n * n; });
    ints.subscribe(
        [](int v)
        { printf("OnNext: %d\n", v); },
        []()
        { printf("OnCompleted\n"); });

    intsFromMap.subscribe(
        [](int v)
        { printf("OnNext: %d\n", v); },
        []()
        { printf("OnCompleted\n"); });
}