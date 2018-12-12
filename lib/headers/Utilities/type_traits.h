#ifndef __TYPE_TRAITS_H
#define __TYPE_TRAITS_H

namespace md_vortex
{

namespace tt
{
  
template <typename T>
  struct is_vec2d : std::false_type {};
template <typename T>
  struct is_vec2d<Vec2d<T>> : std::true_type {};

template <class , class = std::void_t<> >
  struct has_force : std::false_type {};
template <class T>
  struct has_force<T, std::void_t<typename T::Force>> : std::true_type {};

template <class , class = std::void_t<> >
  struct has_energy : std::false_type {};
template <class T>
  struct has_energy<T, std::void_t<typename T::Energy>> : std::true_type {};

}; // namespace tt

}; // namespace md_vortex

#endif /* __TYPE_TRAITS_H */
