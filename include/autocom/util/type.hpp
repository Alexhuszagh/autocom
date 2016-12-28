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
class PointerWrapper
{
    typedef T* P;
    typedef PointerWrapper<T> This;

    P p = nullptr;

public:
    PointerWrapper() = default;
    PointerWrapper(const This&) = default;
    This & operator=(const This&) = default;
    PointerWrapper(This &&) = default;
    This & operator=(This &&) = default;

    PointerWrapper(P &p);
    PointerWrapper(T &t);

    // DATA
    explicit operator P() const;
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
PointerWrapper<T>::PointerWrapper(P &p):
    p(p)
{}


/** \brief Construct from moved type.
 */
template <typename T>
PointerWrapper<T>::PointerWrapper(T &t):
    p(&t)
{}


/** \brief Convert explicitly to the type.
 */
template <typename T>
PointerWrapper<T>::operator P() const
{
    return p;
}

// MACROS
// ------


/** \brief Wraps an object to be passed by value.
 *
 *  Wraps non-array objects via inheritance. Inheritance is prevent
 *  implicit conversion of similar types, using a different class
 *  definition for each type.
 *
 *  The value is copied into DispParams.
 */
#define AUTOCOM_WRAP_VALUE(T, Name)                                     \
    class Name: public ValueWrapper<T>                                  \
    {                                                                   \
    protected:                                                          \
        typedef ValueWrapper<T> Base;                                   \
                                                                        \
    public:                                                             \
        typedef T type;                                                 \
        using Base::Base;                                               \
    }


/** \brief Wraps an object to be passed by pointer.
 *
 *  Wraps non-array objects via inheritance. Inheritance is prevent
 *  implicit conversion of similar types, using a different class
 *  definition for each type.
 *
 *  The pointer is copied into DispParams.
 */
#define AUTOCOM_WRAP_REFERENCE(T, Name)                                 \
    class Name: public PointerWrapper<T>                                \
    {                                                                   \
    protected:                                                          \
        typedef PointerWrapper<T> Base;                                 \
                                                                        \
    public:                                                             \
        typedef T* type;                                                \
        using Base::Base;                                               \
    }


/** \brief Wraps an object to be passed by pointer.
 */
#define AUTOCOM_WRAP_POINTER(T, Name)                                   \
    AUTOCOM_WRAP_REFERENCE(T, Name##Pointer);                           \
    typedef Name##Pointer Name##Ptr


/** \brief Wraps an object to be passed by double pointer.
 *
 *  The pointer to the pointer is copied into DispParams.
 */
#define AUTOCOM_WRAP_DOUBLE_POINTER(T, Name)                            \
    AUTOCOM_WRAP_POINTER(T*, Name##Double);                             \
    typedef Name##DoublePtr Name##DblPtr;                               \
    typedef Name##DoublePointer Name##DblPointer;


/** \brief Generic type wrapper via macro.
 */
#define AUTOCOM_WRAPPER(T, Name)                                        \
    AUTOCOM_WRAP_VALUE(T, Name);                                        \
    AUTOCOM_WRAP_POINTER(T, Name);                                      \
    AUTOCOM_WRAP_DOUBLE_POINTER(T, Name)


// TYPES
// -----

AUTOCOM_WRAP_VALUE(std::nullptr_t, SafeNull);
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
AUTOCOM_WRAPPER(BSTR, SafeBstr);
AUTOCOM_WRAPPER(CURRENCY, SafeCurrency);
AUTOCOM_WRAPPER(SCODE, SafeError);
AUTOCOM_WRAPPER(DATE, SafeDate);
AUTOCOM_WRAPPER(LONGLONG, SafeLongLong);
AUTOCOM_WRAPPER(ULONGLONG, SafeULongLong);
AUTOCOM_WRAPPER(DECIMAL, SafeDecimal);
AUTOCOM_WRAPPER(IUnknown*, SafeIUnknown);
AUTOCOM_WRAPPER(IDispatch*, SafeIDispatch);

#ifdef HAVE_PROPSYS
    AUTOCOM_WRAPPER(LARGE_INTEGER, SafeLargeInteger);
    AUTOCOM_WRAPPER(ULARGE_INTEGER, SafeULargeInteger);
    AUTOCOM_WRAPPER(FILETIME, SafeFiletime);
    AUTOCOM_WRAPPER(CLSID, SafeClsid);
    AUTOCOM_WRAPPER(GUID, SafeGuid);
    AUTOCOM_WRAPPER(CLIPDATA, SafeClipData);
    AUTOCOM_WRAPPER(IStream, SafeIStream);
    AUTOCOM_WRAPPER(IStream, SafeIStreamObject);
    AUTOCOM_WRAPPER(IStorage, SafeIStorage);
    AUTOCOM_WRAPPER(IStorage, SafeIStorageObject);
    AUTOCOM_WRAPPER(LPVERSIONEDSTREAM, SafeLpVersionedStream);
    AUTOCOM_WRAPPER(Bstr, SafeBlob);
    AUTOCOM_WRAPPER(Bstr, SafeBlobObject);
    AUTOCOM_WRAPPER(LPSTR, SafeLpstr);
    AUTOCOM_WRAPPER(LPWSTR, SafeLpwstr);
#endif      // HAVE_PROPSYS


// CLEANUP
// -------

#undef AUTOCOM_WRAP_VALUE
#undef AUTOCOM_WRAP_REFERENCE
#undef AUTOCOM_WRAP_POINTER
#undef AUTOCOM_WRAP_DOUBLE_POINTER
#undef AUTOCOM_WRAPPER

}   /* autocom */
