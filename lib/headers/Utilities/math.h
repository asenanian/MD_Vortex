#ifndef __MATH_H
#define __MATH_H

#include "requires.h"

namespace md_vortex
{

namespace math_detail
{

template <typename T, int N, typename = std::nullptr_t>
  struct pow;

template <typename T, int N>
  struct pow<T,N,requires<(N>0)>>
{
    inline static constexpr decltype(auto) apply (const T& t)
    { return t * pow<T, N - 1>::apply(t); }
};

template <typename T, int N>
  struct pow<T,N,requires<(N<0)>>
{
    inline static constexpr decltype(auto) apply (const T& t)
    { return static_cast<T>(1) / (t * pow<T, - N - 1>::apply(t)); }
};

template <typename T>
  struct pow<T,0>
{
    inline static constexpr decltype(auto) apply (const T&)
    { return static_cast<T>(1.); }
};

template <typename T>
inline constexpr decltype(auto) polynomial (const T& var, const T& last)
{
    return last;
}

template <typename T, typename ...Tail>
inline constexpr decltype(auto) polynomial(const T& var,
                                           const T& first,
                                           Tail ...tail)
{
    return first*pow<T,sizeof...(tail)>::apply(var)
            + polynomial(var,tail...);
}

}; // namespace math_detail

template <int N, typename T>
inline constexpr decltype(auto) const_pow (const T& t)
{ return math_detail::pow<T,N>::apply(t); }

template <typename T, typename ...Tail>
inline constexpr decltype(auto) polynomial(const T& t, Tail ...tail)
{ return math_detail::polynomial(t,tail...); }


}; // namespace md_vortex

#endif /* __MATH_H */
