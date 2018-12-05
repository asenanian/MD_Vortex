#ifndef __VORTEX_H
#define __VORTEX_H

#include <optional>

#include "common.h"

namespace md_vortex
{
    class Vortex
    {
    public:
        const static double T0_PENETRATION_DEPTH;
        const static double T0_COHERENCE_LENGTH;

        using Temperature = std::optional<TemperatureMap>;
        static Temperature temperature;

        Vortex (const int&, const int&) noexcept;
        Vortex (const PositionVector&) noexcept;
        ~Vortex ();

        void AddForce (const ForceVector&);
          // queues forces to act on vortex
        void AddForce (const double&, const double&);
          // queues forces to act on vortex
        void UpdatePositions (const double&);
          // applies forces in queue
        void Move (const PositionVector&);
          // moves the vortex by an amount given by the argument

        // getters
        double get_inv_penetration_depth () const;
        double get_inv_coherence_length () const;
        const PositionVector& get_pos () const;
        const ForceVector& get_force () const;

    private:
        PositionVector * pos;
        ForceVector * force;
          // typedefs are defined in common.h

        double inv_penetration_depth;
        double inv_coherence_length;
          // superconducting params
    };
};

#endif /* end of include guard: __VORTEX_H */
