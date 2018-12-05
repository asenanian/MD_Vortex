#ifndef __UTILITIES_H
#define __UTILITIES_H

#include <map>
#include <iostream>
#include <vector>
#include <cmath>

float ran1(long *idum);
inline int ran1(const int &min, const int &max,long *idum)
{return (max - min)*ran1(idum) + min;}

namespace utils
{
    template <typename Range,typename Function>
    Function for_each (Range & range, Function func)
    {
        return std::for_each (range.begin(),range.end(),func);
    }

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
            if (pred(*begin))
                func(*begin);
        return func;
    }

    constexpr double mod (double x, double mod)
    {
        return !mod ? x : std::fmod(std::fmod(x,mod) + mod,mod);
    }
}

namespace requires_detail
{
    template <bool B>
    struct requires_imp
    {
        using template_error_type_failed_to_meet_requirements_on_template_parameters =
      std::nullptr_t;
    };

    template <>
    struct requires_imp <false> {};
}; // namespace requires_detail

template <bool B>
using requires = typename requires_detail::requires_imp<B>
  ::template_error_type_failed_to_meet_requirements_on_template_parameters;
#endif /* __UTILITIES_H */
