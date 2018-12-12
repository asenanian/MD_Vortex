#ifndef __REQUIRES_H
#define __REQUIRES_H

namespace md_vortex
{

namespace requires_detail
{

template <bool B>
  struct requires_imp
{
    using template_error_type_failed_to_meet_requirements_on_template_parameters =
  std::nullptr_t;
};

template <>
struct requires_imp <false> {};

}; // namespace requires_detail

template <bool B>
using requires = typename requires_detail::requires_imp<B>
  ::template_error_type_failed_to_meet_requirements_on_template_parameters;

}; // namespace md_vortex

#endif /* __REQUIRES_H */
