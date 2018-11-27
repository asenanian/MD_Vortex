#ifndef __COMMON_H
#define __COMMON_H

// forward declarations
template <typename T> struct Vec2d;
struct BoundaryCondition;
class Vortex;

// typedefs
typedef std::vector<Vortex*> Lattice;
typedef std::vector<const BoundaryCondition*> BoundaryConditions;
typedef Vec2d<double> position_vector;
typedef Vec2d<double> force_vector;

#endif /* __COMMON_H */
