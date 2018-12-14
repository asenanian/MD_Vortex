#ifndef __TEMPERATURE_MAP_H
#define __TEMPERATURE_MAP_H

#include "vec2d.h"
#include "Utilities/math.h" // constexpr polynomial

namespace md_vortex
{

namespace /* helper */
// defines a two helper classes: Index, and gen_seq.
// Index is a list of ints, and gen_seq generates an Index
{
    template <std::size_t... Is>
      struct Index {};

    template <std::size_t N, std::size_t... Is>
      struct gen_seq : gen_seq<N-1,N-1,Is...> {};

    template <std::size_t... Is>
      struct gen_seq<0,Is...> : Index<Is...> {};
}

template <typename ...Ts>
  class TemperatureMap
// Temperature map is a Functor which is a function over the real numbers
// returning the temperature for a given Vec2d<double>. The function is
// a polynomial of order sizeof...(Ts). The polynomial is set at compile
// time, so the arguments to the constructor must be rvalue.
{
public:
    TemperatureMap(Ts&&... args)
      : args(std::forward<Ts>(args)...)
    {
        // Do nothing
    }

    inline double operator ()(const PositionVector& pos)
    {
        return func(args,pos.x);
    }

private:
    std::tuple<Ts...> args;

    template <typename... Args, std::size_t... Is>
    inline double func(std::tuple<Args...>& tup, Index<Is...>,const double& var)
    {
        return math_detail::polynomial(var,std::get<Is>(tup)...);
    }

    template <typename... Args>
    inline double func(std::tuple<Args...>& tup, const double& var)
    {
        return func(tup,gen_seq<sizeof...(Args)>{},var);
    }

};

}; // namespace md_vortex

#endif /* __TEMPERATURE_MAP_H */
