//----------- Scan.cpp
#include "rxcpp/rx.hpp"
#include <iostream>
int main()
{
    int count = 0;
    auto values = rxcpp::observable<>::range(1, 20).scan(
        0,
        [&count](int seed, int v)
        {
            count++;
            return seed + v;
        });
    values.subscribe(
        [&](int v)
        { printf("values[%f]/count[%d] : Average through Scan: %f\n", (double)v, count, (double)v / count); },
        []()
        { printf("OnCompleted\n"); });
}