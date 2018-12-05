#ifndef __COMMON_H
#define __COMMON_H

namespace md_vortex
{
    template <typename T> struct Vec2d;
    class Vortex;
    struct TemperatureMap;
    namespace bc
    { struct BoundaryCondition; };

    typedef Vec2d<double> PositionVector;
    typedef Vec2d<double> ForceVector;
    typedef std::vector<std::unique_ptr<const ForceVector>> Forces;
    typedef std::vector<std::shared_ptr<const bc::BoundaryCondition>> BoundaryConditions;
    typedef std::vector<Vortex*> Vortices;

    const double PI                             = 3.14159265358979;
    const double PERMEABILITY                   = 4*PI*100; // T*nm/A
    const double MAG_FLUX_QUANTUM               = 2.067e3; // T*nm*nm

};



#endif /* __COMMON_H */
