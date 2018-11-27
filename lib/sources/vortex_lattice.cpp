#include "utilities.h"
#include "boundary_condition.h"
#include "vortex_lattice.h"
#include "vec2d.h"
#include "vortex.h"

VortexLattice::VortexLattice ()
{
    m_vortices = Lattice();
    m_boundary_conditions = BoundaryConditions();
}

VortexLattice::~VortexLattice ()
{ for (Vortex * vortex : m_vortices) delete vortex; }

void VortexLattice::nucleateVortex (const int & x, const int & y)
{ m_vortices.push_back(new Vortex(x,y)); }

void VortexLattice::nucleateVortex (Vortex * vortex)
{ m_vortices.push_back(vortex); }

void VortexLattice::operator <<(Vortex * vortex)
{ m_vortices.push_back(vortex); }

void VortexLattice::operator <<(std::shared_ptr<bc::BoundaryCondition> boundary_condition)
{ m_boundary_conditions.push_back(boundary_condition); }

void VortexLattice::step (const double & gradT)
{
    for (auto vortex1 = m_vortices.begin(); vortex1 != m_vortices.end(); ++vortex1)
    {
        // vortex-vortex interactions
        for (auto vortex2 = vortex1+1; vortex2 != m_vortices.end(); ++vortex2)
        {
            position_vector seperation = (*vortex1)->get_pos() - (*vortex2)->get_pos();
            force_vector int_force = interactionForce(seperation);

            (*vortex1)->addForce(int_force); (*vortex2)->addForce(-int_force);
        }
        // thermal gradient force
        (*vortex1)->addForce(gradT,0);
    }

    // apply boundary conditions
    utils::for_each(m_vortices,[this](Vortex * vortex){
        for (auto boundary_condition : m_boundary_conditions)
            if (boundary_condition->contains(vortex))
                boundary_condition->apply(vortex);
        vortex->updatePositions();
    });
}

double VortexLattice::energy () const
{
    double energy = 0;
    utils::for_each_pair(m_vortices,[this,&energy](Vortex * v1,Vortex * v2) {
        if (v1 == v2) return;

        position_vector r_12 = v1->get_pos() - v2->get_pos();
        const double dist = norm(r_12);
        energy += interactionEnergy(dist);
    });
    return energy;
}

void VortexLattice::dump () const
{
    utils::for_each(m_vortices,[](Vortex * vortex){
        std::cout << "(" << vortex->get_pos().x << "," << vortex->get_pos().y << ")" << std::endl;
    });
}

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
