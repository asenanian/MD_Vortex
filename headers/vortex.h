#ifndef __VORTEX_H
#define __VORTEX_H

#include "common.h"

class Vortex
{
public:
    Vortex (const int&, const int&);
    Vortex (const position_vector&);
    ~Vortex ();

    const position_vector & getPos () const;
      // returns position of vortex
    void addForce (const force_vector&);
      // queues forces to act on vortex
    void addForce (const double&, const double&);
      // queues forces to act on vortex
    void updatePositions ();
      // applies forces in queue
private:
    position_vector * m_pos;
    force_vector * m_force;
      // typedefs are defined in common.h
};

#endif /* end of include guard: __VORTEX_H */
