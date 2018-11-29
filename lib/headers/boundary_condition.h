#ifndef __BOUNDARY_CONDITION_H
#define __BOUNDARY_CONDITION_H

#include "utilities.h"
#include "common.h"

namespace bc
{
    enum Orientation { HORIZONTAL, VERTICAL };
      // orientation of boundary conditions
    enum Comparator { LESS_THAN, GREATER_THAN };
      // helpful/simple specifier to determine relative position of vortex to bc.

    //----------------------------------------------------
    struct BoundaryCondition
    {
        BoundaryCondition (const double&,Orientation,Comparator);
          // c'tor with helpful/simple specifiers to determine if vortex is on boundary
        BoundaryCondition (std::function<bool(Vortex*)>);
          // user defined function for deducing if vortex is on boundary
        virtual ~BoundaryCondition () {}
          // default d'tor
        virtual void apply (Vortex * vortex) const = 0;
          // apply action of boundary conditon. See derived classes below.
        std::function<bool(Vortex*)> contains;
          // function to deduce if vortex is on boundary.
        Orientation m_orientation;
    };
    //----------------------------------------------------
    struct ReflectiveBoundaryCondition : public BoundaryCondition
    {
        ReflectiveBoundaryCondition(const double&,Orientation,Comparator);
        void apply (Vortex * vortex) const;
    };
    //----------------------------------------------------
    struct AbsorbingBoundaryCondition : public BoundaryCondition
    {
        AbsorbingBoundaryCondition(const double&,Orientation,Comparator);
        void apply (Vortex * vortex) const;
    };
};

#endif /* __BOUNDARY_CONDITION_H */
