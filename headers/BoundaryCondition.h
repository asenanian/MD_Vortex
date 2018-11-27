#ifndef __BOUNDARY_CONDITION_H
#define __BOUNDARY_CONDITION_H

#include "common.h"

struct BoundaryCondition
{
    enum Orientation { horizontal, vertical };
    enum Dir { left, right };

    BoundaryCondition (const int & p_pos, const Orientation & p_orientation, const Dir & p_dir)
      : m_orientation(p_orientation), m_dir(p_dir), m_pos(p_pos) {}
    virtual ~BoundaryCondition ();
    virtual void apply (Vortex * vortex) const = 0;

    std::function<bool(Vortex*)> contains;

    int m_pos;
    Orientation m_orientation;
    Dir m_dir;
};

struct ReflectiveBoundaryCondition : public BoundaryCondition
{
    //ReflectiveBoundaryCondition () {}
    void apply (Vortex * vortex) {}
};

struct AbsorbingBoundaryCondition : public BoundaryCondition
{
    //AbsorbingBoundaryCondition () {}
    void apply (Vortex * vortex) { delete vortex; }
};

#endif /* __BOUNDARY_CONDITION_H */
