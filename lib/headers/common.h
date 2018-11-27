#ifndef __COMMON_H
#define __COMMON_H

// forward declarations
template <typename T> struct Vec2d;
namespace bc
{ struct BoundaryCondition; }
class Vortex;

// typedefs
typedef Vec2d<double> position_vector;
typedef Vec2d<double> force_vector;
typedef std::vector<Vortex*> Lattice;
typedef std::vector<force_vector*> Forces;
typedef std::vector<const std::shared_ptr<bc::BoundaryCondition>> BoundaryConditions;

#endif /* __COMMON_H */
