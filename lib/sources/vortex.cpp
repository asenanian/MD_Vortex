#include "utilities.h"
#include "vortex.h"
#include "vec2d.h"
#include "temperature_map.h"

using namespace md_vortex;

//-----------------------------------------------------------
Vortex::Vortex (const int &p_x, const int &p_y) noexcept
{
    pos = new PositionVector(p_x,p_y);
    force = new ForceVector(0,0);

    //if (!temperature)
        //temperature = Temperature({0.,0.});

    inv_penetration_depth = sqrt(1. - temperature(*pos))/T0_PENETRATION_DEPTH;
    inv_coherence_length = sqrt(1. - temperature(*pos))/T0_COHERENCE_LENGTH;
}
//-----------------------------------------------------------
Vortex::Vortex (const std::pair<int,int> &p_pos) noexcept
{
    pos = new PositionVector(p_pos.first,p_pos.second);
    force = new ForceVector(0,0);

    //if (!temperature)
        //temperature = Temperature({0.,0.});

    inv_penetration_depth = sqrt(1. - temperature(*pos))/T0_PENETRATION_DEPTH;
    inv_coherence_length = sqrt(1. - temperature(*pos))/T0_COHERENCE_LENGTH;
}
//-----------------------------------------------------------
Vortex::Vortex (const PositionVector &p_pos) noexcept
{
    pos = new PositionVector(p_pos);
    force = new ForceVector(0,0);
    //if (!temperature)
        //temperature = Temperature({0.,0.});

    inv_penetration_depth = sqrt(1. - temperature(*pos))/T0_PENETRATION_DEPTH;
    inv_coherence_length = sqrt(1. - temperature(*pos))/T0_COHERENCE_LENGTH;
}
//-----------------------------------------------------------
Vortex::~Vortex ()
{
    delete pos;
    delete force;
}
//-----------------------------------------------------------
void Vortex::addForce (const ForceVector & ext_force)
{
    *force += ext_force;
}
//-----------------------------------------------------------
void Vortex::addForce (const double & f_x, const double &f_y)
{
    force->x += f_x;
    force->y += f_y;
}
//-----------------------------------------------------------
void Vortex::updatePositions (const double &time_step)
{
    pos->x += force->x*time_step;
    pos->y += force->y*time_step;
    *force = ForceVector(0,0);
}
//-----------------------------------------------------------
void Vortex::move (const PositionVector & dr)
{
    *pos += dr;
    *force = ForceVector(0,0);
    inv_penetration_depth = sqrt(1. - temperature(*pos))/T0_PENETRATION_DEPTH;
    inv_coherence_length = sqrt(1. - temperature(*pos))/T0_COHERENCE_LENGTH;
}
//-----------------------------------------------------------
double Vortex::get_inv_penetration_depth () const
{
    return inv_penetration_depth;
}
//-----------------------------------------------------------
double Vortex::get_inv_coherence_length() const
{
    return inv_coherence_length;
}
//-----------------------------------------------------------
const PositionVector & Vortex::get_pos () const
{
    return *pos;
}
//-----------------------------------------------------------
const ForceVector & Vortex::get_force () const
{
    return *force;
}
