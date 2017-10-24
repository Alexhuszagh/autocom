//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief Type aliases based on vartype.
 *
 *  Literal overloads for integer types, and VARTYPE-based type
 *  wrappers.
 */

#include <autocom/util/alias.h>

#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable:4244)
#endif          // MSVC


namespace autocom
{
namespace literals
{
// LITERALS
// --------

PutChar operator"" _I1(char value)
{
    return PutChar(value);
}


PutChar operator"" _I1(unsigned long long int value)
{
    return PutChar(value);
}


PutUChar operator"" _UI1(char value)
{
    return PutUChar(value);
}


PutUChar operator"" _UI1(unsigned long long int value)
{
    return PutUChar(value);
}


PutShort operator"" _I2(unsigned long long int value)
{
    return PutShort(value);
}


PutUShort operator"" _UI2(unsigned long long int value)
{
    return PutUShort(value);
}


PutInt operator"" _INT(unsigned long long int value)
{
    return PutInt(value);
}


PutUInt operator"" _UINT(unsigned long long int value)
{
    return PutUInt(value);
}


PutLong operator"" _I4(unsigned long long int value)
{
    return PutLong(value);
}


PutULong operator"" _UI4(unsigned long long int value)
{
    return PutULong(value);
}


PutLongLong operator"" _I8(unsigned long long int value)
{
    return PutLongLong(value);
}


PutULongLong operator"" _UI8(unsigned long long int value)
{
    return PutULongLong(value);
}


PutFloat operator"" _R4(unsigned long long int value)
{
    return PutFloat(value);
}


PutFloat operator"" _R4(long double value)
{
    return PutFloat(value);
}


PutDouble operator"" _R8(unsigned long long int value)
{
    return PutDouble(value);
}


PutDouble operator"" _R8(long double value)
{
    return PutDouble(value);
}


PutBool operator"" _BOOL(unsigned long long int value)
{
    return PutBool(value);
}


PutDate operator"" _DATE(unsigned long long int value)
{
    return PutDate(value);
}


PutDate operator"" _DATE(long double value)
{
    return PutDate(value);
}


PutError operator"" _ERROR(unsigned long long int value)
{
    return PutError(value);
}


}   /* literals */


namespace wrappers
{
// ALIASES
// -------


/** \brief Overload type-safe aliases by value.
 *
 *  Defines 4 overloads for the given type, Put/Get with P_ and G_
 *  prefixes, and Put/Get with the W_ prefix (for wrap).
 */
#define AUTOCOM_VALUE_ALIASER(Type, Name, Vt)                           \
    Put##Name P_##Vt(Type &&t)                                          \
    {                                                                   \
        return Put##Name(std::move(t));                                 \
    }                                                                   \
                                                                        \
    Get##Name G_##Vt(Type &t)                                           \
    {                                                                   \
        return Get##Name(t);                                            \
    }                                                                   \
                                                                        \
    Put##Name W_##Vt(Type &&t)                                          \
    {                                                                   \
        return Put##Name(std::move(t));                                 \
    }                                                                   \
                                                                        \
    Get##Name W_##Vt(Type &t)                                           \
                                                                        \
    {                                                                   \
        return Get##Name(t);                                            \
    }


/** \brief Overload type-safe aliases by pointer.
 */
#define AUTOCOM_POINTER_ALIASER(Type, Name, Vt)                         \
    AUTOCOM_VALUE_ALIASER(Type*, Name##Ptr, Vt)

/** \brief Overload type-safe aliases.
 */
#define AUTOCOM_ALIASER(Type, Name, Vt)                                 \
    AUTOCOM_VALUE_ALIASER(Type, Name, Vt)                               \
    AUTOCOM_POINTER_ALIASER(Type, Name, Vt)


// GENERIC
AUTOCOM_ALIASER(BSTR, Bstr, BSTR)
AUTOCOM_ALIASER(CHAR, Char, I1)
AUTOCOM_ALIASER(UCHAR, UChar, UI1)
AUTOCOM_ALIASER(SHORT, Short, I2)
AUTOCOM_ALIASER(USHORT, UShort, UI2)
AUTOCOM_ALIASER(INT, Int, INT)
AUTOCOM_ALIASER(UINT, UInt, UINT)
AUTOCOM_ALIASER(LONG, Long, I4)
AUTOCOM_ALIASER(ULONG, ULong, UI4)
AUTOCOM_ALIASER(LONGLONG, LongLong, I8)
AUTOCOM_ALIASER(ULONGLONG, ULongLong, UI8)
AUTOCOM_ALIASER(FLOAT, Float, R4)
AUTOCOM_ALIASER(DOUBLE, Double, R8)
AUTOCOM_ALIASER(VARIANT_BOOL, Bool, BOOL)
AUTOCOM_ALIASER(DATE, Date, DATE)
AUTOCOM_ALIASER(SCODE, Error, ERROR)
AUTOCOM_ALIASER(CURRENCY, Currency, CY)
AUTOCOM_ALIASER(SAFEARRAY*, SafeArray, ARRAY)
AUTOCOM_ALIASER(IUnknown*, IUnknown, UNKNOWN)
AUTOCOM_ALIASER(IDispatch*, IDispatch, DISPATCH)
AUTOCOM_ALIASER(DECIMAL, Decimal, DECIMAL)
AUTOCOM_VALUE_ALIASER(VARIANT*, Variant, VARIANT)

// CLEANUP
// -------

#undef AUTOCOM_VALUE_ALIASER
#undef AUTOCOM_POINTER_ALIASER
#undef AUTOCOM_ALIASER

}   /* wrappers */
}   /* autocom */

#ifdef _MSC_VER
#   pragma warning(pop)
#endif          // MSVC
