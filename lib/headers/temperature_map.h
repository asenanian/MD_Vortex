#ifndef __TEMPERATURE_MAP_H
#define __TEMPERATURE_MAP_H

#include <list>

#include "vec2d.h"

namespace md_vortex
{
    struct TemperatureMap
    {
        std::list<double> coeffs;

        TemperatureMap(std::initializer_list<double> p_coeffs)
          : coeffs(p_coeffs) {}

        double at(const PositionVector & pos) const
        {
            double temp = 0;
            int power = 0;
            for (auto coeff : coeffs)
            {
                temp += coeff*pow(pos.x,power);
                power++;
            }
            return temp;
        }

        TemperatureMap(const TemperatureMap & p_temperature_map)
        {
            coeffs = p_temperature_map.coeffs;
        }
    };
};

#endif /* __TEMPERATURE_MAP_H */
