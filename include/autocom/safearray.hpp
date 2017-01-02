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
    SafeArrayBound(const size_t size);
    SafeArrayBound(const size_t lower,
        const size_t upper);

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
    SafeArray();
    ~SafeArray();
//    SafeArray(const SafeArray&) = default;
//    SafeArray & operator=(const SafeArray&) = default;
//    SafeArray(SafeArray&&) = default;
//    SafeArray & operator=(SafeArray&&) = default;

    // INITIALIZERS
    void create(UINT dimensions,
        SafeArrayBound *bound);
    void close();

    // DATA
    size_t size(const size_t size = 0) const;
    // TODO: iterator....

//    SAFEARRAY * copy() const;

//    void * access();
//    void unaccess();
//    void * get(LONG *indices);
//    void put(LONG *indices,
//        void *data);
//    void resize(SafeArrayBound *bound);
};

// DOWNCASTING
// -----------

static_assert(sizeof(SafeArrayBound) == sizeof(SAFEARRAYBOUND), "sizeof(SafeArrayBound) != sizeof(SAFEARRAYBOUND), cannot safely downcast");
static_assert(sizeof(SafeArray<INT>) == sizeof(SAFEARRAY), "sizeof(SafeArray) != sizeof(SAFEARRAY), cannot safely downcast");

// IMPLEMENTATION
// --------------


/** \brief Create empty array.
 */
template <typename T>
SafeArray<T>::SafeArray()
{
    SafeArrayBound bound(0);
    create(1, &bound);
}


/** \brief Destructor.
 */
template <typename T>
SafeArray<T>::~SafeArray()
{
    close();
}


/** \brief Create array.
 */
template <typename T>
void SafeArray<T>::create(UINT dimensions,
    SafeArrayBound *bound)
{
    auto *array = SafeArrayCreate(vt, dimensions, (bound));
    if (!array) {
        throw std::runtime_error("Unhandled exception in SafeArrayCreate, maybe out of memory?\n");
    }
    reinterpret_cast<SAFEARRAY&>(*this) = *array;
}


/** \brief Destroy array.
 */
template <typename T>
void SafeArray<T>::close()
{
    SafeArrayDestroy(this);
}


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


/** \brief Write implementation for constexpr.
 */
template <typename T>
constexpr VARTYPE SafeArray<T>::vt;


}   /* autocom */
