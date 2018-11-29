#include "utilities.h"
#include "vortex.h"
#include "vec2d.h"
//-----------------------------------------------------------
Vortex::Vortex (const int &p_x, const int &p_y)
{
    m_pos = new position_vector(p_x,p_y);
    m_force = new force_vector();
}
//-----------------------------------------------------------
Vortex::Vortex (const position_vector &p_pos)
{
    m_pos = new position_vector(p_pos);
    m_force = new force_vector();
}
//-----------------------------------------------------------
Vortex::~Vortex ()
{
    delete m_pos;
    delete m_force;
}
//-----------------------------------------------------------
void Vortex::addForce (const force_vector & force)
{
    *m_force += force;
}
//-----------------------------------------------------------
void Vortex::addForce (const double & f_x, const double &f_y)
{
    m_force->x += f_x;
    m_force->y += f_y;
}
//-----------------------------------------------------------
void Vortex::updatePositions ()
{
    m_pos->x += m_force->x*VISCOSITY;
    m_pos->y += m_force->y*VISCOSITY;
}
void Vortex::clearForces ()
{
    *m_force = force_vector(0,0);
}
//-----------------------------------------------------------
const position_vector & Vortex::get_pos () const
{
    return *m_pos;
}
//-----------------------------------------------------------
const force_vector & Vortex::get_force () const
{
    return *m_force;
}
