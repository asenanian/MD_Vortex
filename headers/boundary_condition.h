#ifndef __BOUNDARY_CONDITION_H
#define __BOUNDARY_CONDITION_H

#include "utilities.h"
#include "common.h"

namespace bc
{
    enum Orientation { HORIZONTAL, VERTICAL };
    enum Direction { LESS, GREATER };

    struct BoundaryCondition
    {
        BoundaryCondition (const double&,Direction,Orientation);
        virtual ~BoundaryCondition () {}
        virtual void apply (Vortex * vortex) const = 0;

        std::function<bool(Vortex*)> contains;
    };

    struct ReflectiveBoundaryCondition : public BoundaryCondition
    {
        ReflectiveBoundaryCondition(const double&,Direction,Orientation);
        void apply (Vortex * vortex) const;
    };

    struct AbsorbingBoundaryCondition : public BoundaryCondition
    {
        AbsorbingBoundaryCondition(const double&,Direction,Orientation);
        void apply (Vortex * vortex) const;
    };
};

#endif /* __BOUNDARY_CONDITION_H */
