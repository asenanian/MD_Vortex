#ifndef __VORTEX_LATTICE_H
#define __VORTEX_LATTICE_H

#include "common.h"
#include "Utilities/type_traits.h"
#include "Utilities/ranges.h"
#include "vortex.h"

namespace md_vortex
{

template <typename S>
  class VortexLattice
{
public:
    using Forces =  std::vector<std::unique_ptr<const ForceVector>>;
    using BoundaryConditions = std::vector<std::shared_ptr<const BoundaryCondition>>;
    using Vortices =  std::vector<Vortex*>;

    VortexLattice () noexcept;
        // no temperature dependence
    VortexLattice (const VortexLattice&) = delete;
        // disable copy constructor
    ~VortexLattice ();
        // d'tor
    VortexLattice<S>& operator <<(Vortex*);
        // add constructed vortex
    VortexLattice<S>& operator <<(std::shared_ptr<const BoundaryCondition>);
        // add constructed vortex
    VortexLattice<S>& operator <<(std::unique_ptr<const ForceVector>);
        // add constructed vortex
    std::tuple<size_t,double> equilibrate (const double& /*tolerance*/, double& /*time_step*/);
        // evolve the system until steady state given by parameter tolerance is reached
    void step (const double& /*time_step*/);
        // dynamic step due to interactions and external forces
    void dump (std::ostream&) const;
        // output positions of vortices
    double energy () const;
        // combined energy of lattice due to interactions
    void clearVortices();
        // clear lattice of vortices

private:
    Vortices vortices;
        // container of vortices
    BoundaryConditions boundary_conditions;
        // container of user-specified boundary conditions
    Forces external_forces;
        // container of external forces
};

//-------------------------------------------------
template <typename S>
VortexLattice<S>::VortexLattice () noexcept
{
    static_assert(tt::has_force<S>::value,
        "Stepper needs a Force template parameter.");
    static_assert(tt::has_energy<S>::value,
        "Stepper needs a Energy template parameter.");
    vortices = Vortices();
    boundary_conditions = BoundaryConditions();
    external_forces = Forces();
}
//-------------------------------------------------
template <typename S>
VortexLattice<S>::~VortexLattice ()
{
    for (auto vortex : vortices)
        delete vortex;
    vortices.clear();
}
template <typename S>
//-------------------------------------------------
VortexLattice<S>& VortexLattice<S>::operator <<(Vortex * vortex)
{
    vortices.push_back(vortex);
    return *this;
}
//-------------------------------------------------
template <typename S>
VortexLattice<S>& VortexLattice<S>:: operator <<(std::unique_ptr<const ForceVector> force)
{
    external_forces.push_back(std::move(force));
    return *this;
}
//-------------------------------------------------
template <typename S>
VortexLattice<S>& VortexLattice<S>::operator <<(std::shared_ptr<const BoundaryCondition> boundary_condition)
{
    boundary_conditions.push_back(boundary_condition);
    return *this;
}
//-------------------------------------------------
template <typename S>
std::tuple<size_t,double> VortexLattice<S>::equilibrate (const double& tolerance, double& time_step)
{
    double energy = this->energy();
    size_t n_attempts = 0;
    size_t steps = 0;

    for (;;)
    {
        steps++;
        step(time_step);
        if ( steps % 300 == 0 )
        {
            double new_energy = this->energy();
            double delta_E = energy - new_energy;
            if (delta_E/energy < tolerance && delta_E >= 0)
            {
                time_step = time_step < 1 ? time_step*1.2 : 1.;
                return {n_attempts,delta_E};
            }
            else
            {
                //std::cout << "failed " << delta_E << std::endl;
                time_step = time_step > 1e-04 ? time_step/1.2 : 1e-04;
                n_attempts++;
                energy = new_energy;
            }
        }
    }
}
//-------------------------------------------------
template <typename S>
void VortexLattice<S>::step (const double& time_step)
{
    for (auto vortex1 = vortices.begin(); vortex1 != vortices.end(); ++vortex1)
    {
        for (auto vortex2 = vortex1+1; vortex2 != vortices.end(); ++vortex2)
        // vortex-vortex interactions
        {
            ForceVector int_force = S::Force::apply(*vortex1,*vortex2);

            (*vortex1)->addForce(int_force);
            (*vortex2)->addForce(-int_force);
        }

        for (const auto& external_force : external_forces)
        // external forces
        {
            (*vortex1)->addForce(*external_force);
        }

        for (const auto & boundary_condition : boundary_conditions)
        // apply boundary conditions
        {
            if (boundary_condition->contains((*vortex1)->get_pos() + S::step(*vortex1,time_step)))
            {
                boundary_condition->apply(*vortex1);
            }
        }
        // update positions based on forces calculated above
        PositionVector dr = S::step(*vortex1,time_step);
        (*vortex1)->move(dr);
    }
}
//-------------------------------------------------
template <typename S>
double VortexLattice<S>::energy () const
{
    double energy = 0;
    ranges::for_each_pair(vortices,[this,&energy]
      (Vortex * v1, Vortex * v2) {
        if (v1 == v2)
            return;
        energy += S::Energy::apply(v1,v2);
    });
    
    energy /= 2.;
    ranges::for_each(vortices,[this,&energy](Vortex * v1){
        double lambda_inv = v1->get_inv_penetration_depth();
        energy += lambda_inv*lambda_inv*MAG_FLUX_QUANTUM*MAG_FLUX_QUANTUM/(4*PI*PERMEABILITY);
    });
    return energy;
}
//-------------------------------------------------
template <typename S>
void VortexLattice<S>::dump (std::ostream& out) const
{
    ranges::for_each(vortices,[&out](Vortex * vortex){
        out << vortex->get_pos()/Vortex::T0_PENETRATION_DEPTH << std::endl;
    });
}
//-------------------------------------------------
template <typename S>
void VortexLattice<S>::clearVortices()
{
    for (auto vortex : vortices)
        delete vortex;
    vortices.clear();
}

}; // namespace md_vortex

#endif /* __VORTEX_LATTICE_H */
