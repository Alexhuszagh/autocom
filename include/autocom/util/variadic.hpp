//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup AutoCom
 *  \brief Variadic template utilities.
 */

#pragma once

#include <tuple>
#include <type_traits>


namespace autocom
{
// OBJECTS
// -------


/** \brief Get value at index from variadic pack.
 */
template <
    size_t N,
    typename... Ts
>
decltype(auto) packGet(Ts&&... ts) noexcept
{
    return std::get<N>(std::forward_as_tuple(std::forward<Ts>(ts)...));
}

}   /* autocom */
