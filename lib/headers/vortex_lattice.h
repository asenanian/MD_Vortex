#ifndef __VORTEX_LATTICE_H
#define __VORTEX_LATTICE_H

#include "common.h"

class VortexLattice
{
public:
    VortexLattice ();
        // construct lattice
    VortexLattice (const VortexLattice&) = delete;
        // disable copy constructor
    ~VortexLattice ();
        // d'tor
    VortexLattice& operator <<(Vortex * vortex);
        // add constructed vortex
    VortexLattice& operator <<(std::shared_ptr<const bc::BoundaryCondition>);
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
    force_vector interactionForce (position_vector&) const;
        // functional form of interaction force f(r)
    double interactionEnergy (const double&) const;
        // vortex-vortex interaction energy
    Lattice m_vortices;
        // container of vortices
    BoundaryConditions m_boundary_conditions;
        // container of user-specified boundary conditions
};


#endif /* __VORTEX_LATTICE_H */
