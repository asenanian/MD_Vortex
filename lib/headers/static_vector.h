#ifndef __STATIC_VECTOR_H
#define __STATIC_VECTOR_H

#include <type_traits>

#include "utilities.h"

namespace md_vortex
{

struct StaticVector
{
    // constructors
    StaticVector (const size_t size) noexcept;
    StaticVector (const size_t size, const double value) noexcept;
    template <typename T, requires<std::is_same<T,double>::value> = nullptr>
    StaticVector (std::initializer_list<T> list) noexcept;

    // copy/movers
    StaticVector (const StaticVector& rhs);
    StaticVector& operator =(const StaticVector& rhs);
    StaticVector (StaticVector&& rhs) noexcept;
    StaticVector& operator =(StaticVector&& rhs) noexcept;

    // data members
    std::unique_ptr<double[],decltype(&free)> data{nullptr,&free};
    size_t size;
};

StaticVector::StaticVector (const size_t size) noexcept
  : data(size > 0 ? static_cast<double*>(malloc(size * sizeof(double)))
                  : nullptr,
          &free), size(size)
{
    std::fill(data.get(),data.get() + size,
            std::numeric_limits<double>::signalingNaN());
}

StaticVector::StaticVector (const size_t size, const double value) noexcept
  : data(size > 0 ? static_cast<double*>(malloc(size * sizeof(double)))
                  : nullptr,
          &free), size(size)
{
    std::fill(data.get(),data.get() + size,value);
}

template <typename T, requires<std::is_same<T,double>::value> = nullptr>
StaticVector::StaticVector (std::initializer_list<T> list) noexcept
  : data(list.size() > 0 ? static_cast<double*> (
                              malloc(list.size() * sizeof(double)))
                          : nullptr,
         &free), size(list.size())
{
    std::copy(list.begin(),list.end(),data.get());
}

StaticVector::StaticVector (const StaticVector& rhs)
  : data(rhs.size > 0 ? static_cast<double*> (
                            malloc(rhs.size * sizeof(double)))
                          : nullptr,
                          &free), size(rhs.size)
{
    std::copy(rhs.data.get(),rhs.data.get() + rhs.size, data.get());
}

StaticVector::StaticVector (StaticVector&& rhs) noexcept
{
    data = std::move(rhs.data);
}

StaticVector& StaticVector::operator =(const StaticVector& rhs)
{
    if (this != &rhs)
    {
        if (size != rhs.size)
        {
            data.reset(rhs.size() > 0 ? static_cast<double*> (
                                          malloc(rhs.size() + sizeof(double)))
                                          : nullptr);
        }
        std::copy(rhs.data.get(),rhs.data.get() + rhs.size,data.get());
    }
    return *this;
}

StaticVector& StaticVector::operator =(StaticVector&& rhs) noexcept
{
    if (this != &ths)
    {
        data = std::move(rhs.data);
    }
    return *this;
}

}; // namespace md_vortex
#endif /*__STATIC_VECTOR_H */
