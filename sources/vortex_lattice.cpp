#include "utilities.h"
#include "vortex_lattice.h"
#include "vec2d.h"
#include "vortex.h"

VortexLattice::VortexLattice (const int & p_nx, const int & p_ny)
    : m_nx(p_nx), m_ny(p_ny)
{ m_vortices = Lattice(); }

VortexLattice::~VortexLattice ()
{ for (Vortex * vortex : m_vortices) delete vortex; }

void VortexLattice::step (const double & gradT)
{
    for (auto vortex1 = m_vortices.begin(); vortex1 != m_vortices.end(); ++vortex1)
    {
        // vortex-vortex interactions
        for (auto vortex2 = vortex1+1; vortex2 != m_vortices.end(); ++vortex2)
        {
            position_vector seperation = (*vortex1)->getPos() - (*vortex2)->getPos();
            force_vector int_force = interactionForce(seperation);

            (*vortex1)->addForce(int_force); (*vortex2)->addForce(-int_force);
        }
        // thermal gradient force
        (*vortex1)->addForce(gradT,0);
    }

    // apply boundary conditions
    utils::for_each(m_vortices,std::mem_fn(&Vortex::updatePositions));
}

double VortexLattice::energy () const
{
    double energy = 0;
    utils::for_each_pair(m_vortices,[this,&energy](Vortex * v1,Vortex * v2) {
        if (v1 == v2) return;

        position_vector r_12 = v1->getPos() - v2->getPos();
        const double dist = norm(r_12);
        energy += interactionEnergy(dist);
    });
    return energy;
}

void VortexLattice::dump () const
{
    utils::for_each(m_vortices,[](Vortex * vortex){
        std::cout << "(" << vortex->getPos().x << "," << vortex->getPos().y << ")" << std::endl;
    });
}

void VortexLattice::nucleateVortex (const int & x, const int & y)
{ m_vortices.push_back(new Vortex(x,y)); }

///////////////////
// private methods
inline
double VortexLattice::interactionEnergy (const double & seperation) const
{ return -INTERACTION_STRENGTH/seperation;}

inline
force_vector VortexLattice::interactionForce (position_vector& seperation) const
{
    double r_mag = norm(seperation);
    force_vector force = seperation*INTERACTION_STRENGTH/(r_mag*r_mag*r_mag);
    return force;
}
