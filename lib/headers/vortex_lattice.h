#ifndef __VORTEX_LATTICE_H
#define __VORTEX_LATTICE_H

#include "common.h"
#include "utilities.h"

class VortexLattice
{
public:
    VortexLattice ();
        // construct lattice
    ~VortexLattice ();
        // d'tor
    void nucleateVortex (const int&, const int&);
        // vortex constructor at site i,j on Tc2 strip
    void nucleateVortex (Vortex * vortex);
        // add constructed vortex
    void operator <<(Vortex * vortex);
        // add constructed vortex
    void operator <<(const std::shared_ptr<bc::BoundaryCondition>);
        // add constructed vortex
    void step (const double&);
        // dynamic step due to interactions and gradient
    void dump () const;
        // output positions of vortices
    double energy () const;
        // combined energy of lattice due to interactions

    using iterator = Lattice::iterator;
    iterator begin () { return m_vortices.begin(); }
    iterator end () { return m_vortices.end(); }
private:
    double interactionEnergy (const double&) const;
        // vortex-vortex interaction energy
    force_vector interactionForce (position_vector&) const;
        // functional form of interaction force f(r)
    Lattice m_vortices;
        // container of vortices
    BoundaryConditions m_boundary_conditions;
        // container of user-specified boundary conditions
};


#endif /* __VORTEX_LATTICE_H */
