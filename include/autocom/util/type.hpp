//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCom
 *  \brief Strong type wrappers for WinAPI data types.
 *
 *  Type wrappers that are not implicitly convertible, even among
 *  aliases, for use with dispparams.
 */

#pragma once

#include "bstr.hpp"

#include <type_traits>
#include <utility>


namespace autocom
{
// OBJECTS
// -------


/** \brief Type wrapper for passing by as an R-value.
 *
 *  \warning The object is copied (or moved) into the container
 *  and to the variant.
 */
template <typename T>
class RValueWrapper
{
protected:
    typedef RValueWrapper<T> This;

    T t = T();

public:
    RValueWrapper() = default;
    RValueWrapper(const This&) = default;
    This & operator=(const This&) = default;
    RValueWrapper(This &&) = default;
    This & operator=(This &&) = default;

    RValueWrapper(const T &t);
    RValueWrapper(T &&t);

    // DATA
    explicit operator T() const;
};


/** \brief Type wrapper for passing by reference.
 *
 *  Only accept arguments by reference, to ensure the argument passed
 *  must outlived the lifetime of the caller.
 */
template <typename T>
class LValueWrapper
{
protected:
    typedef T& R;
    typedef LValueWrapper<T> This;

    R r;

public:
    LValueWrapper() = delete;
    LValueWrapper(const This&) = default;
    This & operator=(const This&) = default;
    LValueWrapper(This &&) = default;
    This & operator=(This &&) = default;

    LValueWrapper(R r);

    // DATA
    explicit operator R() const;
};

// TODO: need a vector and array....


// IMPLEMENTATION
// --------------


/** \brief Construct from copy of type.
 */
template <typename T>
RValueWrapper<T>::RValueWrapper(const T &t):
    t(t)
{}


/** \brief Construct from moved type.
 */
template <typename T>
RValueWrapper<T>::RValueWrapper(T &&t):
    t(std::move(t))
{}


/** \brief Convert explicitly to the type.
 */
template <typename T>
RValueWrapper<T>::operator T() const
{
    return t;
}


/** \brief Construct from copy of type.
 */
template <typename T>
LValueWrapper<T>::LValueWrapper(R r):
    r(r)
{}



/** \brief Convert explicitly to the type.
 */
template <typename T>
LValueWrapper<T>::operator R() const
{
    return r;
}

// MACROS
// ------


/** \brief Wrap object to be passed as an R-value.
 *
 *  Wraps non-array objects via inheritance. Inheritance is prevent
 *  implicit conversion of similar types, using a different class
 *  definition for each type.
 *
 *  The value is forwarded and copied into the variant.
 */
#define AUTOCOM_WRAP_RVALUE(T, Name)                                     \
    class Name: public RValueWrapper<T>                                 \
    {                                                                   \
    protected:                                                          \
        typedef RValueWrapper<T> Base;                                  \
                                                                        \
    public:                                                             \
        typedef T type;                                                 \
        using Base::Base;                                               \
    }


/** \brief Wrap object to be passed as an L-value.
 *
 *  Wraps non-array objects via inheritance. Inheritance is prevent
 *  implicit conversion of similar types, using a different class
 *  definition for each type.
 *
 *  The reference is forwarded and assigned to from the variant, or
 *  copied into the variant.
 */
#define AUTOCOM_WRAP_LVALUE(T, Name)                                    \
    class Name: public LValueWrapper<T>                                 \
    {                                                                   \
    protected:                                                          \
        typedef LValueWrapper<T> Base;                                  \
                                                                        \
    public:                                                             \
        typedef T& type;                                                \
        using Base::Base;                                               \
    }


/** \brief Wrap object pointer to be passed as an R-value.
 */
#define AUTOCOM_WRAP_RVALUE_POINTER(T, Name)                            \
    AUTOCOM_WRAP_RVALUE(T*, Name##Pointer);                             \
    typedef Name##Pointer Name##Ptr

/** \brief Wrap object pointer to be passed as an L-value.
 */
#define AUTOCOM_WRAP_LVALUE_POINTER(T, Name)                            \
    AUTOCOM_WRAP_LVALUE(T*, Name##Pointer);                             \
    typedef Name##Pointer Name##Ptr

/** \brief Wrap object.
 */
#define AUTOCOM_WRAP_VALUE(T, Name)                                     \
    AUTOCOM_WRAP_RVALUE(T, Put##Name);                                  \
    AUTOCOM_WRAP_LVALUE(T, Get##Name);

/** \brief Wrap object pointer.
 */
#define AUTOCOM_WRAP_POINTER(T, Name)                                   \
    AUTOCOM_WRAP_RVALUE_POINTER(T, Put##Name);                          \
    AUTOCOM_WRAP_LVALUE_POINTER(T, Get##Name);

/** \brief Generic type wrapper via macro.
 */
#define AUTOCOM_WRAPPER(T, Name)                                        \
    AUTOCOM_WRAP_VALUE(T, Name);                                        \
    AUTOCOM_WRAP_POINTER(T, Name);


// TYPES
// -----

AUTOCOM_WRAP_RVALUE(std::nullptr_t, PutNull);
AUTOCOM_WRAPPER(VARIANT_BOOL, Bool);
AUTOCOM_WRAPPER(CHAR, Char);
AUTOCOM_WRAPPER(UCHAR, UChar);
AUTOCOM_WRAPPER(SHORT, Short);
AUTOCOM_WRAPPER(USHORT, UShort);
AUTOCOM_WRAPPER(INT, Int);
AUTOCOM_WRAPPER(UINT, UInt);
AUTOCOM_WRAPPER(LONG, Long);
AUTOCOM_WRAPPER(ULONG, ULong);
AUTOCOM_WRAPPER(FLOAT, Float);
AUTOCOM_WRAPPER(DOUBLE, Double);
AUTOCOM_WRAPPER(BSTR, Bstr);
AUTOCOM_WRAPPER(CURRENCY, Currency);
AUTOCOM_WRAPPER(SCODE, Error);
AUTOCOM_WRAPPER(DATE, Date);
AUTOCOM_WRAPPER(LONGLONG, LongLong);
AUTOCOM_WRAPPER(ULONGLONG, ULongLong);
AUTOCOM_WRAPPER(DECIMAL, Decimal);
AUTOCOM_WRAPPER(IUnknown*, IUnknown);
AUTOCOM_WRAPPER(IDispatch*, IDispatch);

// CLEANUP
// -------

#undef AUTOCOM_WRAP_RVALUE
#undef AUTOCOM_WRAP_LVALUE
#undef AUTOCOM_WRAP_RVALUE_POINTER
#undef AUTOCOM_WRAP_LVALUE_POINTER
#undef AUTOCOM_WRAP_VALUE
#undef AUTOCOM_WRAP_POINTER
#undef AUTOCOM_WRAPPER

}   /* autocom */
