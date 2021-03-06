#ifndef __INTERACTIONS_H
#define __INTERACTIONS_H

#include "common.h"
#include "vec2d.h"
#include "vortex.h"
#include "utilities.h"

namespace md_vortex
{
namespace interactions
{

struct TempDependentForce
{
    static ForceVector apply(Vortex* vortex1, Vortex * vortex2)
    {
        PositionVector seperation = vortex1->get_pos() - vortex2->get_pos();
        double r_mag = norm(seperation);
        PositionVector unit_vector = seperation/r_mag;

        double lambda_effective = 2./(vortex1->get_inv_penetration_depth() + vortex2->get_inv_penetration_depth());
          // calculate average penetration depth of interacting vortices

        double alpha = r_mag/lambda_effective;
        double alpha_sqrt = sqrt(2.*alpha/PI);
        double alpha_exp = exp(-alpha);
        constexpr double T0_LINE_EN = MAG_FLUX_QUANTUM*MAG_FLUX_QUANTUM/(4*PI*PERMEABILITY);
        double m_strength = T0_LINE_EN/(lambda_effective*lambda_effective);

        ForceVector force = unit_vector*(m_strength*alpha_exp*(1.+2.*alpha)/(2.*alpha_sqrt*alpha))/lambda_effective;
        return force;
    }
};

struct TempDependentEnergy
{
    static double apply(Vortex* vortex1, Vortex * vortex2)
    {
        double r_mag = norm(vortex1->get_pos() - vortex2->get_pos());

        double lambda_effective = 2./(vortex1->get_inv_penetration_depth() + vortex2->get_inv_penetration_depth());
          // calculate average penetration depth of interacting vortices

        double alpha = r_mag/lambda_effective;
        double alpha_sqrt = sqrt(2.*alpha/PI);
        double alpha_exp = exp(-alpha);
        constexpr double T0_LINE_EN = MAG_FLUX_QUANTUM*MAG_FLUX_QUANTUM/(4*PI*PERMEABILITY);
        double m_strength = T0_LINE_EN/(lambda_effective*lambda_effective);

        return m_strength*alpha_exp/alpha_sqrt;
    }
};

}; // namespace interactions
}; // namespace md_vortex

#endif /* __INTERACTIONS_H */
