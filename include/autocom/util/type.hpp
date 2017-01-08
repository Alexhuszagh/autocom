//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief Strong type wrappers for WinAPI data types.
 */

#pragma once

#include <wtypes.h>

#include <type_traits>
#include <utility>


namespace autocom
{
// FORWARD
// -------

struct Bstr;
struct Variant;

template <typename T>
struct SafeArray;

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

    RValueWrapper(const T &t) noexcept;
    RValueWrapper(T &&t) noexcept;

    // DATA
    explicit operator T() const noexcept;

    template <typename U = T>
    typename std::enable_if<std::is_arithmetic<U>::value, RValueWrapper<U>>::type
    operator+() const;

    template <typename U = T>
    typename std::enable_if<std::is_arithmetic<U>::value, RValueWrapper<U>>::type
    operator-() const;
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

    LValueWrapper(R r) noexcept;

    // DATA
    explicit operator R() const noexcept;
};


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

AUTOCOM_WRAP_VALUE(std::nullptr_t, Null);
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
AUTOCOM_WRAPPER(SAFEARRAY*, SafeArray);
AUTOCOM_WRAPPER(VARIANT*, Variant);

// SPECIALIZATION
// --------------


/** \brief Specialize type for VariantType.
 */
#define AUTOCOM_SPECIALIZE(type, vartype)                               \
    template<>                                                          \
    struct VariantType<type, false>                                     \
    {                                                                   \
        static constexpr VARTYPE vt = vartype;                          \
    };                                                                  \
                                                                        \
    template<>                                                          \
    struct VariantType<type, true>                                      \
    {                                                                   \
        static constexpr VARTYPE vt = vartype;                          \
    }

/** \brief Specialize type by value for VariantType.
 */
#define AUTOCOM_VALUE_SPECIALIZER(type, vartype)                        \
    AUTOCOM_SPECIALIZE(type, vartype)

/** \brief Specialize type by pointer for VariantType.
 */
#define AUTOCOM_POINTER_SPECIALIZER(type, vartype)                      \
    AUTOCOM_SPECIALIZE(type*, vartype | VT_BYREF)

/** \brief Specialize type for VariantType.
 */
#define AUTOCOM_SPECIALIZER(type, vartype)                              \
    AUTOCOM_SPECIALIZE(type, vartype);                                  \
    AUTOCOM_POINTER_SPECIALIZER(type, vartype)

/** \brief Specialize type-safe wrapper by value for VariantType.
 */
#define AUTOCOM_SAFE_VALUE_SPECIALIZER(type, vartype)                   \
    AUTOCOM_SPECIALIZE(Put##type, vartype);                             \
    AUTOCOM_SPECIALIZE(Get##type, vartype)

/** \brief Specialize type-safe wrapper by pointer for VariantType.
 */
#define AUTOCOM_SAFE_POINTER_SPECIALIZER(type, vartype)                 \
    AUTOCOM_SAFE_VALUE_SPECIALIZER(type##Ptr, vartype | VT_BYREF)

/** \brief Specialize type-safe wrapper for VariantType.
 */
#define AUTOCOM_SAFE_SPECIALIZER(type, vartype)                         \
    AUTOCOM_SAFE_VALUE_SPECIALIZER(type, vartype);                      \
    AUTOCOM_SAFE_POINTER_SPECIALIZER(type, vartype)


/** \brief Default variant type is user-defined.
 */
template <
    typename T,
    bool IsArray = false
>
struct VariantType
{
    static constexpr VARTYPE vt = VT_USERDEFINED;
};


/** \brief Default variant type is user-defined.
 */
template <typename T>
struct VariantType<T, true>
{
    static constexpr VARTYPE vt = VT_RECORD;
};


/** \brief Specialize SafeArray wrapper.
 */
template <typename T>
struct VariantType<SafeArray<T>, true>
{
    static constexpr VARTYPE vt = VT_ARRAY | VariantType<T, true>::vt;
};


// GENERIC
/** CANNOT specialize the generic SAFEARRAY class, since SafeArrayGetVartype
 *  is the best way to determine the vartype.
 */
AUTOCOM_VALUE_SPECIALIZER(std::nullptr_t, VT_NULL);
AUTOCOM_SPECIALIZER(void, VT_VOID);
AUTOCOM_SPECIALIZER(bool, VT_BOOL);
AUTOCOM_SPECIALIZER(CHAR, VT_I1);
AUTOCOM_SPECIALIZER(UCHAR, VT_UI1);
AUTOCOM_SPECIALIZER(SHORT, VT_I2);
AUTOCOM_SPECIALIZER(USHORT, VT_UI2);
AUTOCOM_SPECIALIZER(INT, VT_INT);
AUTOCOM_SPECIALIZER(UINT, VT_UINT);
AUTOCOM_SPECIALIZER(LONG, VT_I4);
AUTOCOM_SPECIALIZER(ULONG, VT_UI4);
AUTOCOM_SPECIALIZER(LONGLONG, VT_I8);
AUTOCOM_SPECIALIZER(ULONGLONG, VT_UI8);
AUTOCOM_SPECIALIZER(FLOAT, VT_R4);
AUTOCOM_SPECIALIZER(DOUBLE, VT_R8);
AUTOCOM_SPECIALIZER(CURRENCY, VT_CY);
AUTOCOM_SPECIALIZER(BSTR, VT_BSTR);
AUTOCOM_SPECIALIZER(DECIMAL, VT_DECIMAL);
AUTOCOM_SPECIALIZER(IUnknown*, VT_UNKNOWN);
AUTOCOM_SPECIALIZER(IDispatch*, VT_DISPATCH);
AUTOCOM_POINTER_SPECIALIZER(VARIANT, VT_VARIANT);

// WRAPPERS
AUTOCOM_SPECIALIZER(Bstr, VT_BSTR);
AUTOCOM_POINTER_SPECIALIZER(Variant, VT_VARIANT);
AUTOCOM_SAFE_SPECIALIZER(Bool, VT_BOOL);
AUTOCOM_SAFE_SPECIALIZER(Char, VT_I1);
AUTOCOM_SAFE_SPECIALIZER(UChar, VT_UI1);
AUTOCOM_SAFE_SPECIALIZER(Short, VT_I2);
AUTOCOM_SAFE_SPECIALIZER(UShort, VT_UI2);
AUTOCOM_SAFE_SPECIALIZER(Int, VT_INT);
AUTOCOM_SAFE_SPECIALIZER(UInt, VT_UINT);
AUTOCOM_SAFE_SPECIALIZER(Long, VT_I4);
AUTOCOM_SAFE_SPECIALIZER(ULong, VT_UI4);
AUTOCOM_SAFE_SPECIALIZER(Float, VT_R4);
AUTOCOM_SAFE_SPECIALIZER(LongLong, VT_I8);
AUTOCOM_SAFE_SPECIALIZER(ULongLong, VT_UI8);
AUTOCOM_SAFE_SPECIALIZER(Double, VT_R8);
AUTOCOM_SAFE_SPECIALIZER(Bstr, VT_BSTR);
AUTOCOM_SAFE_SPECIALIZER(Currency, VT_CY);
AUTOCOM_SAFE_SPECIALIZER(Error, VT_ERROR);
AUTOCOM_SAFE_SPECIALIZER(Date, VT_DATE);
AUTOCOM_SAFE_SPECIALIZER(IUnknown, VT_UNKNOWN);
AUTOCOM_SAFE_SPECIALIZER(IDispatch, VT_DISPATCH);
AUTOCOM_SAFE_VALUE_SPECIALIZER(Variant, VT_VARIANT | VT_BYREF);
AUTOCOM_SAFE_POINTER_SPECIALIZER(Decimal, VT_DECIMAL);

// IMPLEMENTATION
// --------------


/** \brief Construct from copy of type.
 */
template <typename T>
RValueWrapper<T>::RValueWrapper(const T &t) noexcept:
    t(t)
{}


/** \brief Construct from moved type.
 */
template <typename T>
RValueWrapper<T>::RValueWrapper(T &&t) noexcept:
    t(std::move(t))
{}


/** \brief Convert explicitly to the type.
 */
template <typename T>
RValueWrapper<T>::operator T() const noexcept
{
    return t;
}


/** \brief Positive operator overload for arithmetic types.
 */
template <typename T>
template <typename U>
typename std::enable_if<std::is_arithmetic<U>::value, RValueWrapper<U>>::type
RValueWrapper<T>::operator+() const
{
    return This(+t);
}


/** \brief Negative operator overload for arithmetic types.
 */
template <typename T>
template <typename U>
typename std::enable_if<std::is_arithmetic<U>::value, RValueWrapper<U>>::type
RValueWrapper<T>::operator-() const
{
    return This(-t);
}


/** \brief Construct from copy of type.
 */
template <typename T>
LValueWrapper<T>::LValueWrapper(R r) noexcept:
    r(r)
{}



/** \brief Convert explicitly to the type.
 */
template <typename T>
LValueWrapper<T>::operator R() const noexcept
{
    return r;
}


/** \brief Write implementation for constexpr.
 */
template <
    typename T,
    bool IsArray
>
constexpr VARTYPE VariantType<T, IsArray>::vt;


/** \brief Write implementation for constexpr.
 */
template <typename T>
constexpr VARTYPE VariantType<T, true>::vt;


/** \brief Write implementation for constexpr.
 */
template <typename T>
constexpr VARTYPE VariantType<SafeArray<T>, true>::vt;


// CLEANUP
// -------

#undef AUTOCOM_WRAP_RVALUE
#undef AUTOCOM_WRAP_LVALUE
#undef AUTOCOM_WRAP_RVALUE_POINTER
#undef AUTOCOM_WRAP_LVALUE_POINTER
#undef AUTOCOM_WRAP_VALUE
#undef AUTOCOM_WRAP_POINTER
#undef AUTOCOM_WRAPPER
#undef AUTOCOM_SPECIALIZE
#undef AUTOCOM_VALUE_SPECIALIZER
#undef AUTOCOM_POINTER_SPECIALIZER
#undef AUTOCOM_SPECIALIZER
#undef AUTOCOM_SAFE_VALUE_SPECIALIZER
#undef AUTOCOM_SAFE_POINTER_SPECIALIZER
#undef AUTOCOM_SAFE_SPECIALIZER

}   /* autocom */
