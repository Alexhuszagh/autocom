//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief COM SafeArray wrapper.
 */

#pragma once

#include "util.hpp"

#include <oaidl.h>

#include <memory>


namespace autocom
{
// OBJECTS
// -------


/** \brief C++ wrapper around SAFEARRAYBOUND.
 */
struct SafeArrayBound: SAFEARRAYBOUND
{
    SafeArrayBound() = default;
    SafeArrayBound(const SafeArrayBound&) = default;
    SafeArrayBound & operator=(const SafeArrayBound&) = default;
    SafeArrayBound(SafeArrayBound&&) = default;
    SafeArrayBound & operator=(SafeArrayBound&&) = default;

    SafeArrayBound(const SAFEARRAYBOUND &bound);
    SafeArrayBound(SAFEARRAYBOUND &&bound);

    // DATA
    ULONG size() const;
    LONG & lower();
    ULONG & upper();
};


// TODO: need iterators...


/** \brief C++ wrapper around SAFEARRAY.
 *
 *  Provides an STL-like interface.
 */
template <typename T>
struct SafeArray: SAFEARRAY
{
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    static constexpr VARTYPE vt = VariantType<T, true>::vt;

// TODO: need to fix the constructors...
//    SafeArray() = default;
//    SafeArray(const SafeArray&) = default;
//    SafeArray & operator=(const SafeArray&) = default;
//    SafeArray(SafeArray&&) = default;
//    SafeArray & operator=(SafeArray&&) = default;

    size_t size(const size_t size = 0) const;
    // TODO: iterator....

//    SAFEARRAY * copy() const;
//    void create(VARTYPE vt,
//        UINT dimensions,
//        SafeArrayBound *bound);
//    void close();
//    void * access();
//    void unaccess();
//    void * get(LONG *indices);
//    void put(LONG *indices,
//        void *data);
//    void resize(SafeArrayBound *bound);
};


// IMPLEMENTATION
// --------------


/** \brief Get size of array.
 */
template <typename T>
size_t SafeArray<T>::size(const size_t size) const
{
    if (SAFEARRAY::cDims < 1 || SAFEARRAY::cDims >= size) {
        throw std::out_of_range("SafeArray:: Size requested is out of bounds");
    }

    return SafeArrayBound(SAFEARRAY::rgsabound[size]).size();
}


}   /* autocom */
