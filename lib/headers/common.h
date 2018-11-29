#ifndef __COMMON_H
#define __COMMON_H


template <typename T> struct Vec2d;
struct Space;
class Vortex;
namespace bc
{ struct BoundaryCondition; };

typedef Vec2d<double> position_vector;
typedef Vec2d<double> force_vector;
typedef std::vector<force_vector*> Forces;
typedef std::vector<std::shared_ptr<const bc::BoundaryCondition>> BoundaryConditions;
typedef std::vector<Vortex*> Lattice;



#endif /* __COMMON_H */
