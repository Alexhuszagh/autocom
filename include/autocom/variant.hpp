//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCom
 *  \brief Variant object and collection definitions.
 */

#pragma once

#include "encoding.hpp"
#include "util.hpp"

#include <oaidl.h>
#include <windows.h>
#include <wtypes.h>

#include <initializer_list>
#include <vector>


namespace autocom
{
// FUNCTIONS
// ---------

/** \brief Convert VARIANT data to new type.
 *
 *  \return             Type coercion was successful
 */
bool changeVariantType(VARIANT &variant,
    const VARTYPE vt);

// MACROS -- SETTERS
// -----------------


/** \brief Type-safe class wrapper with move semantics.
 */
#define AUTOCOM_SAFE_MOVE(safe, vartype, field)                         \
                                                                        \
    template <typename VariantType>                                     \
    void setVariant(VariantType &variant,                               \
        safe &&value)                                                   \
    {                                                                   \
        variant.vt = vartype;                                           \
        variant.field = typename safe::type(std::move(value));          \
    }


/** \brief Type-safe class wrapper with copy semantics.
 */
#define AUTOCOM_SAFE_COPY(safe, vartype, field)                         \
                                                                        \
    template <typename VariantType>                                     \
    void setVariant(VariantType &variant,                               \
        safe const &value)                                              \
    {                                                                   \
        variant.vt = vartype;                                           \
        variant.field = typename safe::type(value);                     \
    }


/** \brief Define a setter for a C primitive type.
 */
#define AUTOCOM_SET_PRIMITIVE(type, vartype, field)                     \
                                                                        \
    template <typename VariantType>                                     \
    void setVariant(VariantType &variant,                               \
        type const value)                                               \
    {                                                                   \
        variant.vt = vartype;                                           \
        variant.field = value;                                          \
    }


/** \brief Define a setter for a custom type using move semantics.
 */
#define AUTOCOM_SET_MOVE(type, vartype, field)                          \
                                                                        \
    template <typename VariantType>                                     \
    void setVariant(VariantType &variant,                               \
        type &&value)                                                   \
    {                                                                   \
        variant.vt = vartype;                                           \
        variant.field = std::move(value);                               \
    }


/** \brief Define a setter for a custom type using copy semantics.
 */
#define AUTOCOM_SET_COPY(type, vartype, field)                          \
                                                                        \
    template <typename VariantType>                                     \
    void setVariant(VariantType &variant,                               \
        type const &value)                                              \
    {                                                                   \
        variant.vt = vartype;                                           \
        variant.field = value;                                          \
    }


/** \brief Type-safe wrapper for value setters.
 */
#define AUTOCOM_SAFE_VALUE_SETTER(type, vartype, field)                 \
    AUTOCOM_SAFE_MOVE(Put##type, vartype, field)                        \
    AUTOCOM_SAFE_COPY(Put##type, vartype, field)

/** \brief Type-safe wrapper for pointer setters.
 */
#define AUTOCOM_SAFE_POINTER_SETTER(type, vartype, field)               \
    AUTOCOM_SAFE_VALUE_SETTER(type##Ptr, vartype | VT_BYREF, p##field)

/** \brief Generalized type-safe wrappers for a given type.
 *
 *  Since the type is specialized for TypeWrapper, which provides
 *  a conversion-safe wrapper, any automation type can be passed.
 *
 *  AUTOCOM_SAFE_SETTER(Bool, boolVal, boolVal)
 */
#define AUTOCOM_SAFE_SETTER(type, vartype, field)                       \
    AUTOCOM_SAFE_VALUE_SETTER(type, vartype, field)                     \
    AUTOCOM_SAFE_POINTER_SETTER(type, vartype, field)

/** \brief Generalized dispparams setter overloaded for a primitive type.
 *
 *  AUTOCOM_PRIMITIVE_SETTER(CHAR, VT_I1, cVal)
 */
#define AUTOCOM_PRIMITIVE_SETTER(type, vartype, field)                  \
    AUTOCOM_SET_PRIMITIVE(type, vartype, field)                         \
    AUTOCOM_SET_PRIMITIVE(type*, vartype | VT_BYREF, p##field)

/** \brief Generalized dispparams setter overloaded for a class values.
 *
 *  AUTOCOM_CLASS_VALUE_SETTER(CURRENCY, VT_CY, cyVal)
 */
#define AUTOCOM_CLASS_VALUE_SETTER(type, vartype, field)                \
    AUTOCOM_SET_MOVE(type, vartype, field)                              \
    AUTOCOM_SET_COPY(type, vartype, field)

/** \brief Generalized dispparams setter overloaded for a class values.
 *
 *  AUTOCOM_CLASS_POINTER_SETTER(CURRENCY, VT_CY, cyVal)
 */
#define AUTOCOM_CLASS_POINTER_SETTER(type, vartype, field)              \
    AUTOCOM_SET_MOVE(type*, vartype | VT_BYREF, p##field)               \
    AUTOCOM_SET_COPY(type*, vartype | VT_BYREF, p##field)

/** \brief Generalized dispparams setter overloaded for a class types.
 *
 *  AUTOCOM_CLASS_SETTER(CURRENCY, VT_CY, cyVal)
 */
#define AUTOCOM_CLASS_SETTER(type, vartype, field)                      \
    AUTOCOM_CLASS_VALUE_SETTER(type, vartype, field)                    \
    AUTOCOM_CLASS_POINTER_SETTER(type, vartype, field)

// FUNCTIONS -- SETTERS
// --------------------


/** \brief Set a null parameter.
 */
template <typename VariantType>
void setVariant(VariantType &variant,
    std::nullptr_t value)
{
    variant.vt = VT_NULL;
}


/** \brief Set a null parameter.
 */
template <typename VariantType>
void setVariant(VariantType &variant,
    PutNull value)
{
    variant.vt = VT_NULL;
}


/** \brief Set a BSTR value from copy.
 */
template <typename VariantType>
void setVariant(VariantType &variant,
    const BSTR &value)
{
    variant.vt = VT_BSTR;
    variant.bstrVal = SysAllocStringLen(value, SysStringLen(value));
}


/** \brief Set a BSTR value from move.
 */
template <typename VariantType>
void setVariant(VariantType &variant,
    BSTR &&value)
{
    variant.vt = VT_BSTR;
    variant.bstrVal = std::move(value);
}


/** \brief Set a BSTR value from wrapper.
 */
template <typename VariantType>
void setVariant(VariantType &variant,
    const Bstr &value)
{
    variant.vt = VT_BSTR;
    variant.bstrVal = value.copy();
}


/** \brief Set a pointer to a BSTR.
 */
template <typename VariantType>
void setVariant(VariantType &variant,
    BSTR *value)
{
    variant.vt = VT_BSTR | VT_BYREF;
    variant.pbstrVal = value;
}


/** \brief Set a pointer to BSTR from wrapper.
 */
template <typename VariantType>
void setVariant(VariantType &variant,
    Bstr *value)
{
    variant.vt = VT_BSTR | VT_BYREF;
    variant.pbstrVal = &value->string;
}


/** \brief Set a pointer to an IDispatch pointer.
 */
template <typename VariantType>
void setVariant(VariantType &variant,
    VARIANT *value)
{
    variant.vt = VT_VARIANT | VT_BYREF;
    variant.pvarVal = value;
}


// GENERIC
AUTOCOM_PRIMITIVE_SETTER(CHAR, VT_I1, cVal)
AUTOCOM_PRIMITIVE_SETTER(UCHAR, VT_UI1, bVal)
AUTOCOM_PRIMITIVE_SETTER(SHORT, VT_I2, iVal)
AUTOCOM_PRIMITIVE_SETTER(USHORT, VT_UI2, uiVal)
AUTOCOM_PRIMITIVE_SETTER(INT, VT_INT, intVal)
AUTOCOM_PRIMITIVE_SETTER(UINT, VT_UINT, uintVal)
AUTOCOM_PRIMITIVE_SETTER(LONG, VT_I4, lVal)
AUTOCOM_PRIMITIVE_SETTER(ULONG, VT_UI4, ulVal)
AUTOCOM_PRIMITIVE_SETTER(LONGLONG, VT_I8, llVal)
AUTOCOM_PRIMITIVE_SETTER(ULONGLONG, VT_UI8, ullVal)
AUTOCOM_PRIMITIVE_SETTER(FLOAT, VT_R4, fltVal)
AUTOCOM_PRIMITIVE_SETTER(DOUBLE, VT_R8, dblVal)
AUTOCOM_CLASS_SETTER(CURRENCY, VT_CY, cyVal)
AUTOCOM_CLASS_POINTER_SETTER(DECIMAL, VT_DECIMAL, decVal)
AUTOCOM_CLASS_SETTER(IUnknown*, VT_UNKNOWN, punkVal)
AUTOCOM_CLASS_SETTER(IDispatch*, VT_DISPATCH, pdispVal)
// VT_VECTOR
// VT_ARRAY

// SAFE
AUTOCOM_SAFE_SETTER(Bool, VT_BOOL, boolVal)
AUTOCOM_SAFE_SETTER(Char, VT_I1, cVal)
AUTOCOM_SAFE_SETTER(UChar, VT_UI1, bVal)
AUTOCOM_SAFE_SETTER(Short, VT_I2, iVal)
AUTOCOM_SAFE_SETTER(UShort, VT_UI2, uiVal)
AUTOCOM_SAFE_SETTER(Int, VT_INT, intVal)
AUTOCOM_SAFE_SETTER(UInt, VT_UINT, uintVal)
AUTOCOM_SAFE_SETTER(Long, VT_I4, lVal)
AUTOCOM_SAFE_SETTER(ULong, VT_UI4, ulVal)
AUTOCOM_SAFE_SETTER(Float, VT_R4, fltVal)
AUTOCOM_SAFE_SETTER(Double, VT_R8, dblVal)
AUTOCOM_SAFE_SETTER(LongLong, VT_I8, llVal)
AUTOCOM_SAFE_SETTER(ULongLong, VT_UI8, ullVal)
AUTOCOM_SAFE_SETTER(Bstr, VT_BSTR, bstrVal)
AUTOCOM_SAFE_SETTER(Currency, VT_CY, cyVal)
AUTOCOM_SAFE_SETTER(Error, VT_ERROR, scode)
AUTOCOM_SAFE_SETTER(Date, VT_DATE, date)
AUTOCOM_SAFE_SETTER(IUnknown, VT_UNKNOWN, punkVal)
AUTOCOM_SAFE_SETTER(IDispatch, VT_DISPATCH, pdispVal)
AUTOCOM_SAFE_POINTER_SETTER(Decimal, VT_DECIMAL, decVal)
// VT_VECTOR
// VT_ARRAY

// CLEANUP -- SETTERS
// ------------------

#undef AUTOCOM_SAFE_MOVE
#undef AUTOCOM_SAFE_COPY
#undef AUTOCOM_SET_PRIMITIVE
#undef AUTOCOM_SET_MOVE
#undef AUTOCOM_SET_COPY
#undef AUTOCOM_SAFE_VALUE_SETTER
#undef AUTOCOM_SAFE_POINTER_SETTER
#undef AUTOCOM_SAFE_SETTER
#undef AUTOCOM_PRIMITIVE_SETTER
#undef AUTOCOM_CLASS_VALUE_SETTER
#undef AUTOCOM_CLASS_POINTER_SETTER
#undef AUTOCOM_CLASS_SETTER

// MACROS -- GETTERS
// -----------------


/** \brief Cast value to expected vartype in variant.
 */
#define AUTOCOM_CONVERT_TYPE(variant, vartype)                          \
    if (variant.vt != vartype)   {                                      \
        if (!changeVariantType(variant, vartype)) {                     \
            throw ComFunctionError("VariantChangeType");                \
        }                                                               \
    }                                                                   \


/** \brief Get value from variant.
 */
#define AUTOCOM_GET(type, vartype, field)                               \
                                                                        \
    template <typename VariantType>                                     \
    void getVariant(VariantType &variant,                               \
        type &value)                                                    \
    {                                                                   \
        AUTOCOM_CONVERT_TYPE(variant, vartype)                          \
        value = variant.field;                                         \
    }


/** \brief Get value using type-safe wrapper.
 */
#define AUTOCOM_SAFE_GET(safe, vartype, field)                          \
                                                                        \
    template <typename VariantType>                                     \
    void getVariant(VariantType &variant,                               \
        safe &value)                                                    \
    {                                                                   \
        auto &ref = typename safe::type(value);                         \
        AUTOCOM_CONVERT_TYPE(variant, vartype)                          \
        ref = variant.field;                                            \
    }


/** \brief Define a getter for type by value.
 *
 *  AUTOCOM_VALUE_GETTER(BSTR, VT_BSTR, bstrVal)
 */
#define AUTOCOM_VALUE_GETTER(type, vartype, field)                      \
    AUTOCOM_GET(type, vartype, field)

/** \brief Define a getter for type by reference.
 *
 *  AUTOCOM_POINTER_GETTER(BSTR, VT_BSTR, bstrVal)
 */
#define AUTOCOM_POINTER_GETTER(type, vartype, field)                    \
    AUTOCOM_GET(type*, vartype | VT_BYREF, p##field)

/** \brief Define a getter for type by value and by reference.
 *
 *  AUTOCOM_GETTER(BSTR, VT_BSTR, bstrVal)
 */
#define AUTOCOM_GETTER(type, vartype, field)                            \
    AUTOCOM_VALUE_GETTER(type, vartype, field)                          \
    AUTOCOM_POINTER_GETTER(type, vartype, field)


/** \brief Define a getter for type by value.
 *
 *  AUTOCOM_SAFE_VALUE_GETTER(SafeBool, VT_BOOL, boolVal)
 */
#define AUTOCOM_SAFE_VALUE_GETTER(type, vartype, field)                 \
    AUTOCOM_SAFE_GET(Get##type, vartype, field)

/** \brief Define a getter for type by reference.
 *
 *  AUTOCOM_SAFE_POINTER_GETTER(SafeBool, VT_BOOL, boolVal)
 */
#define AUTOCOM_SAFE_POINTER_GETTER(type, vartype, field)               \
    AUTOCOM_SAFE_VALUE_GETTER(type##Ptr, vartype | VT_BYREF, p##field)

/** \brief Define a getter for type by value and by reference.
 *
 *  AUTOCOM_SAFE_GETTER(SafeBool, VT_BOOL, boolVal)
 */
#define AUTOCOM_SAFE_GETTER(type, vartype, field)                       \
    AUTOCOM_SAFE_VALUE_GETTER(type, vartype, field)                     \
    AUTOCOM_SAFE_POINTER_GETTER(type, vartype, field)

// FUNCTIONS -- GETTERS
// --------------------


/** \brief Get pointer to variant.
 */
template <typename VariantType>
void getVariant(VariantType &variant,
    VARIANT *value)
{
    AUTOCOM_CONVERT_TYPE(variant, VT_VARIANT | VT_BYREF);
    value = variant.pvarVal;
}


/** \brief Get BSTR value in wrapper.
 */
template <typename VariantType>
void getVariant(VariantType &variant,
    Bstr &value)
{
    AUTOCOM_CONVERT_TYPE(variant, VT_BSTR);
    value.clear();
    value.string = variant.bstrVal;
}


/** \brief Get BSTR value pointer in wrapper.
 */
template <typename VariantType>
void getVariant(VariantType &variant,
    Bstr *&value)
{
    AUTOCOM_CONVERT_TYPE(variant, VT_BSTR | VT_BYREF);
    value->clear();
    value->string = *variant.pbstrVal;
}

// GENERIC
AUTOCOM_GETTER(CHAR, VT_I1, cVal)
AUTOCOM_GETTER(UCHAR, VT_UI1, bVal)
AUTOCOM_GETTER(SHORT, VT_I2, iVal)
AUTOCOM_GETTER(USHORT, VT_UI2, uiVal)
AUTOCOM_GETTER(INT, VT_INT, intVal)
AUTOCOM_GETTER(UINT, VT_UINT, uintVal)
AUTOCOM_GETTER(LONG, VT_I4, lVal)
AUTOCOM_GETTER(ULONG, VT_UI4, ulVal)
AUTOCOM_GETTER(FLOAT, VT_R4, fltVal)
AUTOCOM_GETTER(DOUBLE, VT_R8, dblVal)
AUTOCOM_GETTER(LONGLONG, VT_I8, llVal)
AUTOCOM_GETTER(ULONGLONG, VT_UI8, ullVal)
AUTOCOM_GETTER(CURRENCY, VT_CY, cyVal)
AUTOCOM_GETTER(BSTR, VT_BSTR, bstrVal)
AUTOCOM_GETTER(IUnknown*, VT_UNKNOWN, punkVal)
AUTOCOM_GETTER(IDispatch*, VT_DISPATCH, pdispVal)
AUTOCOM_POINTER_GETTER(DECIMAL, VT_DECIMAL, decVal)
// VT_VECTOR
// VT_ARRAY

// SAFE
AUTOCOM_SAFE_GETTER(Bool, VT_BOOL, boolVal)
AUTOCOM_SAFE_GETTER(Char, VT_I1, cVal)
AUTOCOM_SAFE_GETTER(UChar, VT_UI1, bVal)
AUTOCOM_SAFE_GETTER(Short, VT_I2, iVal)
AUTOCOM_SAFE_GETTER(UShort, VT_UI2, uiVal)
AUTOCOM_SAFE_GETTER(Int, VT_INT, intVal)
AUTOCOM_SAFE_GETTER(UInt, VT_UINT, uintVal)
AUTOCOM_SAFE_GETTER(Long, VT_I4, lVal)
AUTOCOM_SAFE_GETTER(ULong, VT_UI4, ulVal)
AUTOCOM_SAFE_GETTER(Float, VT_R4, fltVal)
AUTOCOM_SAFE_GETTER(LongLong, VT_I8, llVal)
AUTOCOM_SAFE_GETTER(ULongLong, VT_UI8, ullVal)
AUTOCOM_SAFE_GETTER(Double, VT_R8, dblVal)
AUTOCOM_SAFE_GETTER(Bstr, VT_BSTR, bstrVal)
AUTOCOM_SAFE_GETTER(Currency, VT_CY, cyVal)
AUTOCOM_SAFE_GETTER(IUnknown, VT_UNKNOWN, punkVal)
AUTOCOM_SAFE_GETTER(IDispatch, VT_DISPATCH, pdispVal)
AUTOCOM_SAFE_POINTER_GETTER(Decimal, VT_DECIMAL, decVal)
// VT_VECTOR
// VT_ARRAY

// CLEANUP -- GETTERS
// ------------------

#undef AUTOCOM_CONVERT_TYPE
#undef AUTOCOM_GET
#undef AUTOCOM_SAFE_GET
#undef AUTOCOM_VALUE_GETTER
#undef AUTOCOM_POINTER_GETTER
#undef AUTOCOM_GETTER
#undef AUTOCOM_SAFE_VALUE_GETTER
#undef AUTOCOM_SAFE_POINTER_GETTER
#undef AUTOCOM_SAFE_GETTER

// OBJECTS
// -------


/** \brief C++ wrapper for VARIANT.
 */
struct Variant: public VARIANT
{
    Variant();
    ~Variant();

    void init();
    void clear();
    bool changeType(const VARTYPE vt);

    template <typename T>
    void set(T &&t);

    template <typename T>
    void get(T &&t);
};


// IMPLEMENTATION
// --------------


/** \brief Set value in variant.
 */
template <typename T>
void Variant::set(T &&t)
{
    setVariant(*this, AUTOCOM_FWD(t));
}


/** \brief Get value from variant.
 */
template <typename T>
void Variant::get(T &&t)
{
    //getVariant(*this, AUTOCOM_FWD(t));
}

// TYPES
// -----

typedef std::vector<Variant> VariantList;

}   /* autocom */
