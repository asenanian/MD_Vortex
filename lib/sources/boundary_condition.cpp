#include "boundary_condition.h"
#include "vortex.h"
#include "vec2d.h"

using namespace bc;
//------------------------------------------------------------------------------
namespace
{
    inline bool containsLessHorizontal(Vortex * vortex, const double & location)
    {
        return vortex->get_pos().y + vortex->get_force().y*VISCOSITY < location;
    }

    inline bool containsLessVertical(Vortex * vortex, const double & location)
    {
        return vortex->get_pos().x + vortex->get_force().x*VISCOSITY < location;
    }

    inline bool containsGreaterHorizontal(Vortex * vortex, const double & location)
    {
        return vortex->get_pos().y + vortex->get_force().y*VISCOSITY > location;
    }

    inline bool containsGreaterVertical(Vortex * vortex, const double & location)
    {
        return vortex->get_pos().x + vortex->get_force().x*VISCOSITY > location;
    }

    typedef std::pair<Orientation,Comparator> BoundaryConditionOption;
    typedef std::function<bool(Vortex*,const double&)> ComparatorFunction;
}
//------------------------------------------------------------------------------
BoundaryCondition::BoundaryCondition(const double & p_pos, Orientation p_orientation,
                                     Comparator p_comparator)
                                     : m_orientation(p_orientation)
{
    //
    auto option_dictionary = std::map<BoundaryConditionOption,ComparatorFunction>{};
    option_dictionary.emplace(BoundaryConditionOption(HORIZONTAL,LESS_THAN),containsLessHorizontal);
    option_dictionary.emplace(BoundaryConditionOption(VERTICAL,LESS_THAN),containsLessVertical);
    option_dictionary.emplace(BoundaryConditionOption(HORIZONTAL,GREATER_THAN),containsGreaterHorizontal);
    option_dictionary.emplace(BoundaryConditionOption(VERTICAL,GREATER_THAN),containsGreaterVertical);

    auto temp_func = option_dictionary[BoundaryConditionOption(p_orientation,p_comparator)];
    contains = std::bind(temp_func,std::placeholders::_1,p_pos);
}
//------------------------------------------------------------------------------
ReflectiveBoundaryCondition::ReflectiveBoundaryCondition (const double & p_pos,
                                                          Orientation p_orientation,
                                                          Comparator p_comparator)
  : BoundaryCondition(p_pos,p_orientation,p_comparator)
{
    // Do nothing
}
//------------------------------------------------------------------------------
void ReflectiveBoundaryCondition::apply (Vortex* vortex) const
{
    switch(m_orientation)
    {
        case Orientation::HORIZONTAL:
        {
            force_vector force = -vortex->get_force();
            force.x = 0;
            vortex->addForce(force);
            break;
        }
        case Orientation::VERTICAL:
        {
            force_vector force = -vortex->get_force();
            force.y = 0;
            vortex->addForce(force);
            break;
        }
    }
    return;
}
//------------------------------------------------------------------------------
void AbsorbingBoundaryCondition::apply (Vortex* vortex) const
{
    // TODO
}
