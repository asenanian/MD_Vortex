#ifndef __RANGES_H
#define __RANGES_H

namespace md_vortex
{
namespace ranges
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

}; // namespace ranges

}; // namespace md_vortex

#endif /* __RANGES_H */
