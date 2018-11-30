#include "utilities.h"
#include "boundary_condition.h"
#include "vortex_lattice.h"
#include "vec2d.h"
#include "vortex.h"
//-------------------------------------------------
VortexLattice::VortexLattice ()
{
    m_vortices = Lattice();
    m_boundary_conditions = BoundaryConditions();
    m_forces = Forces();
}
//-------------------------------------------------
VortexLattice::~VortexLattice ()
{
    for (auto vortex : m_vortices) delete vortex;
}
//-------------------------------------------------
VortexLattice& VortexLattice::operator <<(Vortex * vortex)
{
    m_vortices.push_back(vortex);
    return *this;
}
//-------------------------------------------------
VortexLattice& VortexLattice:: operator <<(std::unique_ptr<const force_vector> force)
{
    m_forces.push_back(std::move(force));
    return *this;
}
//-------------------------------------------------
VortexLattice& VortexLattice::operator <<(std::shared_ptr<const bc::BoundaryCondition> boundary_condition)
{
    m_boundary_conditions.push_back(boundary_condition);
    return *this;
}
//-------------------------------------------------
void VortexLattice::step ()
{
    for (auto vortex1 = m_vortices.begin(); vortex1 != m_vortices.end(); ++vortex1)
    {
      // vortex-vortex interactions
        for (auto vortex2 = vortex1+1; vortex2 != m_vortices.end(); ++vortex2)
        {
            position_vector seperation = (*vortex1)->get_pos() - (*vortex2)->get_pos();
            force_vector int_force = interactionForce(seperation);

            (*vortex1)->addForce(int_force);
            (*vortex2)->addForce(-int_force);
        }
        // external forces
        for (const auto & force : m_forces)
          (*vortex1)->addForce(*force);
    }

    // apply boundary conditions
    utils::for_each(m_vortices,[this](Vortex * vortex){
        for (const auto & boundary_condition : m_boundary_conditions)
            if (boundary_condition->contains(vortex))
                boundary_condition->apply(vortex);
        vortex->updatePositions();
        vortex->clearForces();
    });
}
//-------------------------------------------------
double VortexLattice::energy () const
{
    double energy = 0;
    utils::for_each_pair(m_vortices,[this,&energy]
      (Vortex * v1, Vortex * v2) {
        if (v1 == v2)
            return;

        position_vector r_12 = v1->get_pos() - v2->get_pos();
        const double dist = norm(r_12);
        energy += interactionEnergy(dist);
    });
    return energy;
}
//-------------------------------------------------
void VortexLattice::dump () const
{
    utils::for_each(m_vortices,[](Vortex * vortex){
        std::cout << vortex->get_pos() << std::endl;
    });
}
//-------------------------------------------------

///////////////////////////////////
// private methods
///////////////////////////////////
inline
double VortexLattice::interactionEnergy (const double & r_mag) const
{
    double r_mag_sqrt = sqrt(r_mag);
    double exp_r = exp(-r_mag);
    return INTERACTION_STRENGTH*exp(r_mag)/r_mag_sqrt;
}
//-------------------------------------------------
inline
force_vector VortexLattice::interactionForce (position_vector& seperation) const
{
    double r_mag = norm(seperation);
    double r_mag_sqrt = sqrt(r_mag);
    double exp_r = exp(-r_mag);

    force_vector force = seperation*(INTERACTION_STRENGTH*exp_r*(1.+2.*r_mag)/(2.*r_mag_sqrt*r_mag*r_mag));
    return force;
}
//-------------------------------------------------
