//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief Type aliases based on vartype.
 *
 *  Literal overloads for integer types, and VARTYPE-based type
 *  wrappers.
 */

#pragma once

#include "type.hpp"


namespace autocom
{
namespace literals
{
// LITERALS
// --------

PutChar operator"" _I1(unsigned long long int);
PutUChar operator"" _UI1(unsigned long long int);
PutShort operator"" _I2(unsigned long long int);
PutUShort operator"" _UI2(unsigned long long int);
PutInt operator"" _INT(unsigned long long int);
PutUInt operator"" _UINT(unsigned long long int);
PutLong operator"" _I4(unsigned long long int);
PutULong operator"" _UI4(unsigned long long int);
PutLongLong operator"" _I8(unsigned long long int);
PutULongLong operator"" _UI8(unsigned long long int);
PutFloat operator"" _R4(unsigned long long int);
PutFloat operator"" _R4(long double);
PutDouble operator"" _R8(unsigned long long int);
PutDouble operator"" _R8(long double);
PutBool operator"" _BOOL(unsigned long long int);
PutDate operator"" _DATE(unsigned long long int);
PutDate operator"" _DATE(long double);
PutError operator"" _ERROR(unsigned long long int);

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
    Put##Name P_##Vt(Type &&);                                          \
    Get##Name G_##Vt(Type &);                                           \
    Put##Name W_##Vt(Type &&);                                          \
    Get##Name W_##Vt(Type &)

/** \brief Overload type-safe aliases by pointer.
 */
#define AUTOCOM_POINTER_ALIASER(Type, Name, Vt)                         \
    AUTOCOM_VALUE_ALIASER(Type*, Name##Ptr, Vt)

/** \brief Overload type-safe aliases.
 */
#define AUTOCOM_ALIASER(Type, Name, Vt)                                 \
    AUTOCOM_VALUE_ALIASER(Type, Name, Vt);                              \
    AUTOCOM_POINTER_ALIASER(Type, Name, Vt)


// GENERIC
AUTOCOM_ALIASER(BSTR, Bstr, BSTR);
AUTOCOM_ALIASER(CHAR, Char, I1);
AUTOCOM_ALIASER(UCHAR, UChar, UI1);
AUTOCOM_ALIASER(SHORT, Short, I2);
AUTOCOM_ALIASER(USHORT, UShort, UI2);
AUTOCOM_ALIASER(INT, Int, INT);
AUTOCOM_ALIASER(UINT, UInt, UINT);
AUTOCOM_ALIASER(LONG, Long, I4);
AUTOCOM_ALIASER(ULONG, ULong, UI4);
AUTOCOM_ALIASER(LONGLONG, LongLong, I8);
AUTOCOM_ALIASER(ULONGLONG, ULongLong, UI8);
AUTOCOM_ALIASER(FLOAT, Float, R4);
AUTOCOM_ALIASER(DOUBLE, Double, R8);
AUTOCOM_ALIASER(VARIANT_BOOL, Bool, BOOL);
AUTOCOM_ALIASER(DATE, Date, DATE);
AUTOCOM_ALIASER(SCODE, Error, ERROR);
AUTOCOM_ALIASER(CURRENCY, Currency, CY);
AUTOCOM_ALIASER(SAFEARRAY*, SafeArray, ARRAY);
AUTOCOM_ALIASER(IUnknown*, IUnknown, UNKNOWN);
AUTOCOM_ALIASER(IDispatch*, IDispatch, DISPATCH);
AUTOCOM_ALIASER(DECIMAL, Decimal, DECIMAL);
AUTOCOM_VALUE_ALIASER(VARIANT*, Variant, VARIANT);

// CLEANUP
// -------

#undef AUTOCOM_VALUE_ALIASER
#undef AUTOCOM_POINTER_ALIASER
#undef AUTOCOM_ALIASER

}   /* wrappers */
}   /* autocom */
