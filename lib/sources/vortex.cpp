#include "utilities.h"
#include "vortex.h"
#include "vec2d.h"

Vortex::Vortex (const int &p_x, const int &p_y)
{
    m_pos = new position_vector(p_x,p_y);
    m_force = new force_vector();
}

Vortex::Vortex (const position_vector &p_pos)
{
    m_pos = new position_vector(p_pos);
    m_force = new force_vector();
}

Vortex::~Vortex ()
{ delete m_pos; delete m_force; }

void Vortex::addForce (const force_vector & force)
{ *m_force += force; }

void Vortex::addForce (const double & f_x, const double &f_y)
{
    m_force->x += f_x;
    m_force->y += f_y;
}

void Vortex::updatePositions ()
{
    m_pos->x += m_force->x*VISCOSITY;
    m_pos->y += m_force->y*VISCOSITY;
    if (m_pos->x > RIGHT_WALL) m_pos->x -= m_force->x*VISCOSITY;
    //if (m_pos->x < LEFT_WALL) m_pos->x -= m_force->x*VISCOSITY;
    if (m_pos->y > TOP_WALL) m_pos->y -= m_force->y*VISCOSITY;
    if (m_pos->y < BOTTOM_WALL) m_pos->y -= m_force->y*VISCOSITY;
    m_force->x = 0.; m_force->y = 0.;
}

const position_vector & Vortex::get_pos () const
{ return *m_pos; }
