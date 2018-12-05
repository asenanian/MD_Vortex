#ifndef __TYPE_TRAITS_H
#define __TYPE_TRAITS_H

namespace md_vortex
{

template <typename T>
struct is_vec2d : std::false_type {};
template <typename T>
struct is_vec2d<Vec2d<T>> : std::true_type {};


template <class, class = std::void_t<> >
struct has_type_member : std::false_type {};
template <class T>
struct has_type_member<T, std::void_t<typename T::type>> : std::true_type {};

};

#endif /* __TYPE_TRAITS_H */
