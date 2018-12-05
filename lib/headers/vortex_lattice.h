#ifndef __VORTEX_LATTICE_H
#define __VORTEX_LATTICE_H

#include "common.h"
#include "vortex.h"

namespace md_vortex
{
    template <typename F, typename E>
    struct Interaction
    {
        typedef F Force;
        typedef E Energy;
    };

    class VortexLattice
    {
    public:
        VortexLattice () noexcept;
            // no temperature dependence
        VortexLattice (const VortexLattice&) = delete;
            // disable copy constructor
        ~VortexLattice ();
            // d'tor
        VortexLattice& operator <<(Vortex*);
            // add constructed vortex
        VortexLattice& operator <<(std::shared_ptr<const bc::BoundaryCondition>);
            // add constructed vortex
        VortexLattice& operator <<(std::unique_ptr<const ForceVector>);
            // add constructed vortex
        template <typename InteractionForce, typename Stepper>
        void Step (const double&);
            // dynamic step due to interactions and external forces
        void Dump () const;
            // output positions of vortices
        template <typename InteractionEnergy>
        double Energy () const;
            // combined energy of lattice due to interactions

    private:
        Vortices vortices;
            // container of vortices
        BoundaryConditions boundary_conditions;
            // container of user-specified boundary conditions
        Forces external_forces;
            // container of external forces
    };

//-------------------------------------------------
VortexLattice::VortexLattice () noexcept
{
    vortices = Vortices();
    boundary_conditions = BoundaryConditions();
    external_forces = Forces();
}
//-------------------------------------------------
VortexLattice::~VortexLattice ()
{
    for (auto vortex : vortices)
        delete vortex;
}
//-------------------------------------------------
VortexLattice& VortexLattice::operator <<(Vortex * vortex)
{
    vortices.push_back(vortex);
    return *this;
}
//-------------------------------------------------
VortexLattice& VortexLattice:: operator <<(std::unique_ptr<const ForceVector> force)
{
    external_forces.push_back(std::move(force));
    return *this;
}
//-------------------------------------------------
VortexLattice& VortexLattice::operator <<(std::shared_ptr<const bc::BoundaryCondition> boundary_condition)
{
    boundary_conditions.push_back(boundary_condition);
    return *this;
}
//-------------------------------------------------
template <typename InteractionForce, typename Stepper>
void VortexLattice::Step (const double& time_step)
{
    for (auto vortex1 = vortices.begin(); vortex1 != vortices.end(); ++vortex1)
    {
        for (auto vortex2 = vortex1+1; vortex2 != vortices.end(); ++vortex2)
        // vortex-vortex interactions
        {
            ForceVector int_force = InteractionForce::apply(*vortex1,*vortex2);

            (*vortex1)->AddForce(int_force);
            (*vortex2)->AddForce(-int_force);
        }

        for (const auto& external_force : external_forces)
        // external forces
        {
            (*vortex1)->AddForce(*external_force);
        }

        for (const auto & boundary_condition : boundary_conditions)
        // apply boundary conditions
        {
            if (boundary_condition->Contains((*vortex1)->get_pos() + Stepper::step(*vortex1,time_step)))
            {
                boundary_condition->Apply(*vortex1);
            }
        }
        // update positions based on forces calculated above
        PositionVector dr = Stepper::step(*vortex1,time_step);
        (*vortex1)->Move(dr);
    }
}
//-------------------------------------------------
template <typename InteractionEnergy>
double VortexLattice::Energy () const
{
    double energy = 0;
    utils::for_each_pair(vortices,[this,&energy]
      (Vortex * v1, Vortex * v2) {
        if (v1 == v2)
            return;

        energy += InteractionEnergy::apply(v1,v2);
    });
    return energy;
}
//-------------------------------------------------
void VortexLattice::Dump () const
{
    utils::for_each(vortices,[](Vortex * vortex){
        std::cout << vortex->get_pos() << std::endl;
    });
}
//-------------------------------------------------
    /*
    //-------------------------------------------------
    template <typename Interaction, typename Stepper>
    VortexLattice<Interaction,Stepper>::VortexLattice () noexcept
    {
        vortices = Vortices();
        boundary_conditions = BoundaryConditions();
        external_forces = Forces();
    }
    //-------------------------------------------------
    template <typename Interaction, typename Stepper>
    VortexLattice<Interaction,Stepper>::~VortexLattice ()
    {
        for (auto vortex : vortices)
            delete vortex;
    }
    //-------------------------------------------------
    template <typename Interaction, typename Stepper>
    VortexLattice<Interaction,Stepper>& VortexLattice<Interaction,Stepper>::operator <<(Vortex * vortex)
    {
        vortices.push_back(vortex);
        return *this;
    }
    //-------------------------------------------------
    template <typename Interaction, typename Stepper>
    VortexLattice<Interaction,Stepper>& VortexLattice<Interaction,Stepper>:: operator <<(std::unique_ptr<const ForceVector> force)
    {
        external_forces.push_back(std::move(force));
        return *this;
    }
    //-------------------------------------------------
    template <typename Interaction, typename Stepper>
    VortexLattice<Interaction,Stepper>& VortexLattice<Interaction,Stepper>::operator <<(std::shared_ptr<const bc::BoundaryCondition> boundary_condition)
    {
        boundary_conditions.push_back(boundary_condition);
        return *this;
    }
    //-------------------------------------------------
    template <typename Interaction, typename Stepper>
    void VortexLattice<Interaction,Stepper>::Step (const double& time_step)
    {
        for (auto vortex1 = vortices.begin(); vortex1 != vortices.end(); ++vortex1)
        {
            for (auto vortex2 = vortex1+1; vortex2 != vortices.end(); ++vortex2)
            // vortex-vortex interactions
            {
                ForceVector int_force = Interaction::Force::apply(*vortex1,*vortex2);

                (*vortex1)->AddForce(int_force);
                (*vortex2)->AddForce(-int_force);
            }

            for (const auto& external_force : external_forces)
            // external forces
            {
                (*vortex1)->AddForce(*external_force);
            }

            for (const auto & boundary_condition : boundary_conditions)
            // apply boundary conditions
            {
                if (boundary_condition->Contains((*vortex1)->get_pos() + stepper(*vortex1,time_step)))
                {
                    boundary_condition->Apply(*vortex1);
                }
            }
            // update positions based on forces calculated above
            PositionVector dr = stepper(*vortex1,time_step);
            (*vortex1)->Move(dr);
        }
    }
    //-------------------------------------------------
    template <typename Interaction, typename Stepper>
    double VortexLattice<Interaction,Stepper>::Energy () const
    {
        double energy = 0;
        utils::for_each_pair(vortices,[this,&energy]
          (Vortex * v1, Vortex * v2) {
            if (v1 == v2)
                return;

            energy += Interaction::Energy::apply(v1,v2);
        });
        return energy;
    }
    //-------------------------------------------------
    template <typename Interaction, typename Stepper>
    void VortexLattice<Interaction,Stepper>::Dump () const
    {
        utils::for_each(vortices,[](Vortex * vortex){
            std::cout << vortex->get_pos() << std::endl;
        });
    }
    //-------------------------------------------------
    */
};
#endif /* __VORTEX_LATTICE_H */
