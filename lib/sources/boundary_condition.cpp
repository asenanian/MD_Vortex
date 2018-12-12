#include "boundary_condition.h"
#include "vec2d.h"
#include "vortex.h"

using namespace md_vortex;
//------------------------------------------------------------------------------
namespace
{
    inline bool containsLessHorizontal(const PositionVector &pos, const double & location)
    {
        return pos.y < location;
    }

    inline bool containsLessVertical(const PositionVector &pos,  const double & location)
    {
        return pos.x < location;
    }

    inline bool containsGreaterHorizontal(const PositionVector &pos, const double & location)
    {
        return pos.y > location;
    }

    inline bool containsGreaterVertical(const PositionVector &pos, const double & location)
    {
        return pos.x > location;
    }

    typedef std::pair<Orientation,Comparator> BoundaryConditionOption;
    using ComparatorFunction = std::function<bool(const PositionVector&,const double&)>;
}
//------------------------------------------------------------------------------
BoundaryCondition::BoundaryCondition(const double & p_pos, Orientation p_orientation,
                                     Comparator p_comparator)
                                     : orientation(p_orientation)
{
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
    switch(orientation)
    {
        case Orientation::HORIZONTAL:
        {
            ForceVector force = -vortex->get_force();
            force.x = 0;
            vortex->addForce(force);
            break;
        }
        case Orientation::VERTICAL:
        {
            ForceVector force = -vortex->get_force();
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
