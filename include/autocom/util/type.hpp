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

#include <wtypes.h>

#include <type_traits>
#include <utility>


namespace autocom
{
// OBJECTS
// -------


/** \brief Type wrapper for passing by value.
 *
 *  \warning The object is copied (or moved) into the container
 *  and to the variant.
 */
template <typename T>
class ValueWrapper
{
protected:
    typedef ValueWrapper<T> This;

    T t = T();

public:
    ValueWrapper() = default;
    ValueWrapper(const This&) = default;
    This & operator=(const This&) = default;
    ValueWrapper(This &&) = default;
    This & operator=(This &&) = default;

    ValueWrapper(const T &t);
    ValueWrapper(T &&t);

    // DATA
    explicit operator T() const;
};


/** \brief Type wrapper for passing by reference.
 *
 *  Only accept arguments by reference, to ensure the argument passed
 *  must outlived the lifetime of the caller.
 */
template <typename T>
class RefWrapper
{
    typedef T* P;
    typedef RefWrapper<T> This;

    P p = nullptr;

public:
    RefWrapper() = default;
    RefWrapper(const This&) = default;
    This & operator=(const This&) = default;
    RefWrapper(This &&) = default;
    This & operator=(This &&) = default;

    RefWrapper(P &p);
    RefWrapper(T &t);

    // DATA
    explicit operator P() const;
};


/** \brief Type-safe wrapper for COM string types.
 */
class SafeBstr
{
protected:
    std::wstring string;

public:
    typedef BSTR type;

    SafeBstr() = default;
    SafeBstr(const SafeBstr&) = default;
    SafeBstr & operator=(const SafeBstr&) = default;
    SafeBstr(SafeBstr &&) = default;
    SafeBstr & operator=(SafeBstr &&) = default;

    SafeBstr(const std::string &string);
    SafeBstr(const std::wstring &string);

    // DATA
    explicit operator BSTR() const;
};


/** \brief Type-safe wrapper for COM BStr] references.
 */
class SafeBstrRef
{
protected:
    typedef BSTR* BstrPtr;

    BstrPtr string = nullptr;

public:
    typedef BstrPtr type;

    SafeBstrRef() = default;
    SafeBstrRef(const SafeBstrRef&) = default;
    SafeBstrRef & operator=(const SafeBstrRef&) = default;
    SafeBstrRef(SafeBstrRef &&) = default;
    SafeBstrRef & operator=(SafeBstrRef &&) = default;

    SafeBstrRef(BSTR *&string);
    SafeBstrRef(BSTR &string);

    // DATA
    explicit operator BstrPtr() const;
};


// TODO: need a vector and array....


// IMPLEMENTATION
// --------------


/** \brief Construct from copy of type.
 */
template <typename T>
ValueWrapper<T>::ValueWrapper(const T &t):
    t(t)
{}


/** \brief Construct from moved type.
 */
template <typename T>
ValueWrapper<T>::ValueWrapper(T &&t):
    t(std::move(t))
{}


/** \brief Convert explicitly to the type.
 */
template <typename T>
ValueWrapper<T>::operator T() const
{
    return t;
}



/** \brief Construct from copy of type.
 */
template <typename T>
RefWrapper<T>::RefWrapper(P &p):
    p(p)
{}


/** \brief Construct from moved type.
 */
template <typename T>
RefWrapper<T>::RefWrapper(T &t):
    p(&t)
{}


/** \brief Convert explicitly to the type.
 */
template <typename T>
RefWrapper<T>::operator P() const
{
    return p;
}


// MACROS
// ------


/** \brief Generic type wrapper via macro.
 *
 *  Wraps non-array objects via inheritance. Inheritance is prevent
 *  implicit conversion of similar types, using a different class
 *  definition for each type.
 */
#define AUTOCOM_WRAPPER(T, Name)                                        \
    class Name: public ValueWrapper<T>                                  \
    {                                                                   \
    protected:                                                          \
        typedef ValueWrapper<T> Base;                                   \
                                                                        \
    public:                                                             \
        typedef T type;                                                 \
        using Base::Base;                                               \
    };                                                                  \
                                                                        \
    class Name##Ref: public RefWrapper<T>                               \
    {                                                                   \
    protected:                                                          \
        typedef RefWrapper<T> Base;                                     \
                                                                        \
    public:                                                             \
        typedef T* type;                                                \
        using Base::Base;                                               \
    };                                                                  \
                                                                        \
    class Name##RefRef: public RefWrapper<T*>                           \
    {                                                                   \
    protected:                                                          \
        typedef RefWrapper<T*> Base;                                    \
                                                                        \
    public:                                                             \
        typedef T** type;                                               \
        using Base::Base;                                               \
    };


// TYPES
// -----

AUTOCOM_WRAPPER(VARIANT_BOOL, SafeBool);
AUTOCOM_WRAPPER(CHAR, SafeChar);
AUTOCOM_WRAPPER(UCHAR, SafeUChar);
AUTOCOM_WRAPPER(SHORT, SafeShort);
AUTOCOM_WRAPPER(USHORT, SafeUShort);
AUTOCOM_WRAPPER(INT, SafeInt);
AUTOCOM_WRAPPER(UINT, SafeUInt);
AUTOCOM_WRAPPER(LONG, SafeLong);
AUTOCOM_WRAPPER(ULONG, SafeULong);
AUTOCOM_WRAPPER(FLOAT, SafeFloat);
AUTOCOM_WRAPPER(DOUBLE, SafeDouble);
AUTOCOM_WRAPPER(CURRENCY, SafeCurrency);

// CLEANUP
// -------

#undef AUTOCOM_WRAPPER

}   /* autocom */
