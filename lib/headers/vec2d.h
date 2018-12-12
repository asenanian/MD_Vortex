#ifndef __VEC2D_H
#define __VEC2D_H

#include <type_traits>

#include "Utilities/requires.h"

namespace md_vortex
{

template <typename T>
  struct Vec2d
{
    // constructors
    template <requires<std::is_arithmetic<T>::value> = nullptr>
    Vec2d (const T & p_x, const T & p_y) noexcept  : x(p_x), y(p_y) {}

    template <requires<std::is_arithmetic<T>::value> = nullptr>
    Vec2d (const Vec2d<T> & v) noexcept  : x(v.x), y(v.y) {}

    template <typename U>
    Vec2d (const Vec2d<U> & v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {}

    // vec2d algebra
    template <typename U> Vec2d<T> & operator +=(const Vec2d<U>&);
    template <typename U> Vec2d<T> & operator -=(const Vec2d<U>&);

    // scalar algebra
    Vec2d<T> & operator *=(const T&);
    Vec2d<T> & operator /=(const T&);

    // flip direction
    Vec2d<T> operator -() const;

    T x,y;
};
//----------------------------------------------------------------------
template<typename T>
template<typename U>
Vec2d<T> & Vec2d<T>::operator +=(const Vec2d<U> &rhs)
{
    x += rhs.x;
    y += rhs.y;
    return *this;
}
//----------------------------------------------------------------------
template <typename T>
template <typename U>
Vec2d<T> & Vec2d<T>::operator -=(const Vec2d<U> &rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}
//----------------------------------------------------------------------
template <typename T>
Vec2d<T> & Vec2d<T>::operator *=(const T &rhs)
{
    x *= rhs;
    y *= rhs;
    return *this;
}
//----------------------------------------------------------------------
template <typename T>
Vec2d<T> & Vec2d<T>::operator /=(const T &rhs)
{
    x /= rhs;
    y /= rhs;
    return *this;
}
//----------------------------------------------------------------------
template <typename T>
Vec2d<T> Vec2d<T>::operator -() const
{
    return Vec2d(-x,-y);
}
//----------------------------------------------------------------------

///////////////////////////////////////
// non-member functions
///////////////////////////////////////
template <typename T, typename U>
Vec2d<typename std::common_type<T,U>::type> operator +(const Vec2d<T> &lhs, const Vec2d<U> &rhs)
{
    Vec2d<typename std::common_type<T,U>::type> result(lhs);
    result.x += rhs.x;
    result.y += rhs.y;
    return result;
}
//----------------------------------------------------------------------
template <typename T, typename U>
Vec2d<typename std::common_type<T,U>::type> operator -(const Vec2d<T> &lhs, const Vec2d<U> &rhs)
{
    Vec2d<typename std::common_type<T,U>::type> result(lhs);
    result.x -= rhs.x;
    result.y -= rhs.y;
    return result;
}
//----------------------------------------------------------------------
template <typename T, typename U>
Vec2d<typename std::common_type<T,U>::type> operator *(const Vec2d<T> &lhs, const U &rhs)
{
    Vec2d<typename std::common_type<T,U>::type> result(lhs);
    result *= rhs;
    return result;
}
//----------------------------------------------------------------------
template <typename T, typename U>
Vec2d<typename std::common_type<T,U>::type> operator /(const Vec2d<T> &lhs, const U &rhs)
{
    Vec2d<typename std::common_type<T,U>::type> result(lhs);
    result /= rhs;
    return result;
}
//----------------------------------------------------------------------
template <typename T>
inline bool operator <(const Vec2d<T> &lhs, const Vec2d<T> &rhs)
{
    if (lhs.x != rhs.x)
        return lhs.x < rhs.x;
    else
        return lhs.y < rhs.y;
}
template <typename T>
std::ostream& operator <<(std::ostream &os, const Vec2d<T> &vec)
{
    os << vec.x << "," << vec.y;
    return os;
}
//----------------------------------------------------------------------
template <typename T>
inline double norm(const Vec2d<T> &vec)
{
    return sqrt(vec.x*vec.x + vec.y*vec.y);
}

}; // namespace md_vortex

#endif /* __VEC2D_H */
