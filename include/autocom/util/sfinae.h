//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief Type utilities using SFINAE.
 */

#pragma once

#include <type_traits>


namespace autocom
{
// OBJECTS
// -------

template<class T>
struct DereferenceType
{
    typedef T type;
};


template<class T>
struct DereferenceType<T*>
{
    typedef T type;
};

}   /* autocom */
