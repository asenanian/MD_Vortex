#include "boundary_condition.h"
#include "vortex.h"
#include "vec2d.h"

namespace bc
{
    namespace
    {
        inline bool containsLessHorizontal(Vortex * vortex, const double & location)
        { return vortex->get_pos().y < location; }

        inline bool containsLessVertical(Vortex * vortex, const double & location)
        { return vortex->get_pos().x < location; }

        inline bool containsGreaterHorizontal(Vortex * vortex, const double & location)
        { return vortex->get_pos().y > location; }

        inline bool containsGreaterVertical(Vortex * vortex, const double & location)
        { return vortex->get_pos().x > location; }

        typedef std::pair<Direction,Orientation> BoundaryConditionOption;
        typedef std::function<bool(Vortex*,const double&)> comparator;
    }

    BoundaryCondition::BoundaryCondition(const double & p_pos, Direction p_direction, Orientation p_orientation)
    {
        auto option_dictionary = std::map<BoundaryConditionOption,comparator>{};
        option_dictionary.emplace(BoundaryConditionOption(LESS,HORIZONTAL),containsLessHorizontal);
        option_dictionary.emplace(BoundaryConditionOption(LESS,VERTICAL),containsLessVertical);
        option_dictionary.emplace(BoundaryConditionOption(GREATER,HORIZONTAL),containsGreaterHorizontal);
        option_dictionary.emplace(BoundaryConditionOption(GREATER,VERTICAL),containsGreaterVertical);

        auto temp_func = option_dictionary[BoundaryConditionOption(p_direction,p_orientation)];
        contains = std::bind(temp_func,std::placeholders::_1,p_pos);
    }

    ReflectiveBoundaryCondition::ReflectiveBoundaryCondition (const double & p_pos, Direction p_direction, Orientation p_orientation)
      : BoundaryCondition(p_pos,p_direction,p_orientation) {}

    void ReflectiveBoundaryCondition::apply (Vortex * vortex) const
    { return;}

    void AbsorbingBoundaryCondition::apply (Vortex * vortex) const
    { return;}
};
