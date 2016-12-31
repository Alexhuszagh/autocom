//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup AutoCOM
 *  \brief Macro definitions.
 */

#pragma once

#include <utility>


namespace autocom
{
// MACROS
// ------

#define AUTOCOM_FWD(...) std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

}   /* autocom */
