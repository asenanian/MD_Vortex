#ifndef __COMMON_H
#define __COMMON_H

namespace md_vortex
{
    template <typename T>
      struct Vec2d;

    template <typename ...Args>
      class TemperatureMap;

    class Vortex;
    
    namespace bc
    { struct BoundaryCondition; };

    typedef Vec2d<double> PositionVector;
    typedef Vec2d<double> ForceVector;
    typedef std::function<double(const PositionVector&)> Temperature;

    constexpr const double PI                             = 3.14159265358979;
    constexpr const double PERMEABILITY                   = 4*PI*100; // T*nm/A
    constexpr const double MAG_FLUX_QUANTUM               = 2.067e3; // T*nm*nm

};



#endif /* __COMMON_H */
