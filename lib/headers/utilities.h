#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <map>

#define VISCOSITY 1. // bardeen stephen viscosity at T = 0
#define INTERACTION_STRENGTH 10.
#define RIGHT_WALL 128
#define LEFT_WALL 0
#define TOP_WALL 128
#define BOTTOM_WALL 0

float ran1(long *idum);

namespace utils
{
    template <typename Range,typename Function>
    Function for_each (Range & range, Function func)
    { return std::for_each (range.begin(),range.end(),func); }

    template <typename Range, typename Function>
    Function for_each_pair (Range range, Function func)
    {
        for (typename Range::iterator iter1 = range.begin(); iter1 != range.end(); ++iter1)
            for (typename Range::iterator iter2 = range.begin(); iter2 != range.end(); ++iter2)
                func(*iter1,*iter2);
        return func;
    }

    template <typename Iterator, typename Predicate, typename Function>
    Function for_each_if (Iterator begin, Iterator end, Predicate pred, Function func)
    {
        for (; begin != end; ++begin)
            if (pred(*begin)) func(*begin);
        return func;
    }

    constexpr double mod (double x, double mod)
    { return !mod ? x : std::fmod(std::fmod(x,mod) + mod,mod); }
}
