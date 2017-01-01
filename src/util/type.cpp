//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCOM
 *
 *  Type wrappers that are not implicitly convertible, even among
 *  aliases, for use with dispparams.
 */

#include "autocom.hpp"


namespace autocom
{
// SPECIALIZATION
// --------------


/** \brief Specialize type for VariantType.
 */
#define AUTOCOM_SPECIALIZE(type)                                        \
    constexpr VARTYPE VariantType<type, false>::vt

/** \brief Specialize type by value for VariantType.
 */
#define AUTOCOM_VALUE_SPECIALIZER(type)                                 \
    AUTOCOM_SPECIALIZE(type)

/** \brief Specialize type by pointer for VariantType.
 */
#define AUTOCOM_POINTER_SPECIALIZER(type)                               \
    AUTOCOM_SPECIALIZE(type*)

/** \brief Specialize type for VariantType.
 */
#define AUTOCOM_SPECIALIZER(type)                                       \
    AUTOCOM_SPECIALIZE(type);                                           \
    AUTOCOM_POINTER_SPECIALIZER(type)

/** \brief Specialize type-safe wrapper by value for VariantType.
 */
#define AUTOCOM_SAFE_VALUE_SPECIALIZER(type)                            \
    AUTOCOM_SPECIALIZE(Put##type);                                      \
    AUTOCOM_SPECIALIZE(Get##type)

/** \brief Specialize type-safe wrapper by pointer for VariantType.
 */
#define AUTOCOM_SAFE_POINTER_SPECIALIZER(type)                          \
    AUTOCOM_SAFE_VALUE_SPECIALIZER(type##Ptr)

/** \brief Specialize type-safe wrapper for VariantType.
 */
#define AUTOCOM_SAFE_SPECIALIZER(type)                                  \
    AUTOCOM_SAFE_VALUE_SPECIALIZER(type);                               \
    AUTOCOM_SAFE_POINTER_SPECIALIZER(type)


// GENERIC
/** CANNOT specialize the generic SAFEARRAY class, since SafeArrayGetVartype
 *  is the best way to determine the vartype.
 */
AUTOCOM_VALUE_SPECIALIZER(std::nullptr_t);
AUTOCOM_SPECIALIZER(void);
AUTOCOM_SPECIALIZER(CHAR);
AUTOCOM_SPECIALIZER(UCHAR);
AUTOCOM_SPECIALIZER(SHORT);
AUTOCOM_SPECIALIZER(USHORT);
AUTOCOM_SPECIALIZER(INT);
AUTOCOM_SPECIALIZER(UINT);
AUTOCOM_SPECIALIZER(LONG);
AUTOCOM_SPECIALIZER(ULONG);
AUTOCOM_SPECIALIZER(LONGLONG);
AUTOCOM_SPECIALIZER(ULONGLONG);
AUTOCOM_SPECIALIZER(FLOAT);
AUTOCOM_SPECIALIZER(DOUBLE);
AUTOCOM_SPECIALIZER(CURRENCY);
AUTOCOM_SPECIALIZER(BSTR);
AUTOCOM_SPECIALIZER(DECIMAL);
AUTOCOM_SPECIALIZER(IUnknown*);
AUTOCOM_SPECIALIZER(IDispatch*);
AUTOCOM_POINTER_SPECIALIZER(VARIANT);

// WRAPPERS
AUTOCOM_SPECIALIZER(Bstr);
AUTOCOM_POINTER_SPECIALIZER(Variant);
AUTOCOM_SAFE_SPECIALIZER(Bool);
AUTOCOM_SAFE_SPECIALIZER(Char);
AUTOCOM_SAFE_SPECIALIZER(UChar);
AUTOCOM_SAFE_SPECIALIZER(Short);
AUTOCOM_SAFE_SPECIALIZER(UShort);
AUTOCOM_SAFE_SPECIALIZER(Int);
AUTOCOM_SAFE_SPECIALIZER(UInt);
AUTOCOM_SAFE_SPECIALIZER(Long);
AUTOCOM_SAFE_SPECIALIZER(ULong);
AUTOCOM_SAFE_SPECIALIZER(Float);
AUTOCOM_SAFE_SPECIALIZER(LongLong);
AUTOCOM_SAFE_SPECIALIZER(ULongLong);
AUTOCOM_SAFE_SPECIALIZER(Double);
AUTOCOM_SAFE_SPECIALIZER(Bstr);
AUTOCOM_SAFE_SPECIALIZER(Currency);
AUTOCOM_SAFE_SPECIALIZER(Error);
AUTOCOM_SAFE_SPECIALIZER(Date);
AUTOCOM_SAFE_SPECIALIZER(IUnknown);
AUTOCOM_SAFE_SPECIALIZER(IDispatch);
AUTOCOM_SAFE_VALUE_SPECIALIZER(Variant);
AUTOCOM_SAFE_POINTER_SPECIALIZER(Decimal);

// CLEANUP
// -------

#undef AUTOCOM_SPECIALIZE
#undef AUTOCOM_VALUE_SPECIALIZER
#undef AUTOCOM_POINTER_SPECIALIZER
#undef AUTOCOM_SPECIALIZER
#undef AUTOCOM_SAFE_VALUE_SPECIALIZER
#undef AUTOCOM_SAFE_POINTER_SPECIALIZER
#undef AUTOCOM_SAFE_SPECIALIZER

}   /* autocom */
