//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCom
 *  \brief Variant object and collection definitions.
 */

#include "autocom.hpp"


namespace autocom
{
// MACROS -- SETTERS
// -----------------


/** \brief Type-safe class wrapper with move semantics.
 */
#define AUTOCOM_SAFE_MOVE(safe, vartype, field)                         \
                                                                        \
    void setVariant(VARIANT &variant,                                   \
        safe &&value)                                                   \
    {                                                                   \
        variant.vt = vartype;                                           \
        variant.field = typename safe::type(std::move(value));          \
    }


/** \brief Type-safe class wrapper with copy semantics.
 */
#define AUTOCOM_SAFE_COPY(safe, vartype, field)                         \
                                                                        \
    void setVariant(VARIANT &variant,                                   \
        safe const &value)                                              \
    {                                                                   \
        variant.vt = vartype;                                           \
        variant.field = typename safe::type(value);                     \
    }


/** \brief Define a setter for a C primitive type.
 */
#define AUTOCOM_SET_PRIMITIVE(type, vartype, field)                     \
                                                                        \
    void setVariant(VARIANT &variant,                                   \
        type const value)                                               \
    {                                                                   \
        variant.vt = vartype;                                           \
        variant.field = value;                                           \
    }


/** \brief Define a setter for a custom type using move semantics.
 */
#define AUTOCOM_SET_MOVE(type, vartype, field)                          \
                                                                        \
    void setVariant(VARIANT &variant,                                   \
        type &&value)                                                   \
    {                                                                   \
        variant.vt = vartype;                                           \
        variant.field = std::move(value);                               \
    }


/** \brief Define a setter for a custom type using copy semantics.
 */
#define AUTOCOM_SET_COPY(type, vartype, field)                          \
                                                                        \
    void setVariant(VARIANT &variant,                                   \
        type const &value)                                              \
    {                                                                   \
        variant.vt = vartype;                                           \
        variant.field = value;                                          \
    }


/** \brief Generalized type-safe wrappers for a given type.
 *
 *  Since the type is specialized for TypeWrapper, which provides
 *  a conversion-safe wrapper, any automation type can be passed.
 *
 *  AUTOCOM_SAFE_SETTER(BOOL, boolVal, boolVal)
 */
#define AUTOCOM_SAFE_SETTER(safe, vartype, field)                       \
    AUTOCOM_SAFE_MOVE(safe, vartype, field)                             \
    AUTOCOM_SAFE_COPY(safe, vartype, field)                             \
    AUTOCOM_SAFE_COPY(safe##Ref, vartype | VT_BYREF, p##field)


/** \brief Generalized dispparams setter overloaded for a primitive type.
 *
 *  AUTOCOM_PRIMITIVE_SETTER(CHAR, VT_I1, cVal)
 */
#define AUTOCOM_PRIMITIVE_SETTER(type, vartype, field)                  \
    AUTOCOM_SET_PRIMITIVE(type, vartype, field)                         \
    AUTOCOM_SET_PRIMITIVE(type*, vartype | VT_BYREF, p##field)


/** \brief Generalized dispparams setter overloaded for a primitive type.
 *
 *  AUTOCOM_CLASS_SETTER(CURRENCY, VT_CY, cyVal)
 */
#define AUTOCOM_CLASS_SETTER(type, vartype, field)                      \
    AUTOCOM_SET_MOVE(type, vartype, field)                              \
    AUTOCOM_SET_COPY(type, vartype, field)                              \
    AUTOCOM_SET_MOVE(type*, vartype | VT_BYREF, p##field)               \
    AUTOCOM_SET_COPY(type*, vartype | VT_BYREF, p##field)

// FUNCTIONS -- SETTERS
// --------------------


/** \brief Set a null parameter.
 */
void setVariant(VARIANT &variant,
    std::nullptr_t value)
{
    variant.vt = VT_NULL;
}


/** \brief Set a BSTR value from a std::string.
 */
void setVariant(VARIANT &variant,
    const std::string &value)
{
    setVariant(variant, WIDE(value));
}


/** \brief Set a BSTR value from a std::wstring.
 */
void setVariant(VARIANT &variant,
    const std::wstring &value)
{
    variant.vt = VT_BSTR;
    variant.bstrVal = SysAllocString(value.data());
}


/** \brief Set a pointer to a BSTR.
 */
void setVariant(VARIANT &variant,
    BSTR *value)
{
    variant.vt = VT_BSTR | VT_BYREF;
    variant.pbstrVal = value;
}


/** \brief Set a pointer to an IUnknown instance.
 */
void setVariant(VARIANT &variant,
    IUnknown *value)
{
    variant.vt = VT_UNKNOWN;
    variant.punkVal = value;
}


/** \brief Set a pointer to an IUnknown pointer.
 */
void setVariant(VARIANT &variant,
    IUnknown **value)
{
    variant.vt = VT_UNKNOWN | VT_BYREF;
    variant.ppunkVal = value;
}


/** \brief Set a pointer to an IDispatch instance.
 */
void setVariant(VARIANT &variant,
    IDispatch *value)
{
    variant.vt = VT_DISPATCH;
    variant.pdispVal = value;
}


/** \brief Set a pointer to an IDispatch pointer.
 */
void setVariant(VARIANT &variant,
    IDispatch **value)
{
    variant.vt = VT_DISPATCH | VT_BYREF;
    variant.ppdispVal = value;
}


/** \brief Set a pointer to an IDispatch pointer.
 */
void setVariant(VARIANT &variant,
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
AUTOCOM_PRIMITIVE_SETTER(FLOAT, VT_R4, fltVal)
AUTOCOM_PRIMITIVE_SETTER(DOUBLE, VT_R8, dblVal)
AUTOCOM_CLASS_SETTER(CURRENCY, VT_CY, cyVal)

// SAFE
AUTOCOM_SAFE_SETTER(SafeBool, VT_BOOL, boolVal)
AUTOCOM_SAFE_SETTER(SafeChar, VT_I1, cVal)
AUTOCOM_SAFE_SETTER(SafeUChar, VT_UI1, bVal)
AUTOCOM_SAFE_SETTER(SafeShort, VT_I2, iVal)
AUTOCOM_SAFE_SETTER(SafeUShort, VT_UI2, uiVal)
AUTOCOM_SAFE_SETTER(SafeInt, VT_INT, intVal)
AUTOCOM_SAFE_SETTER(SafeUInt, VT_UINT, uintVal)
AUTOCOM_SAFE_SETTER(SafeLong, VT_I4, lVal)
AUTOCOM_SAFE_SETTER(SafeULong, VT_UI4, ulVal)
AUTOCOM_SAFE_SETTER(SafeFloat, VT_R4, fltVal)
AUTOCOM_SAFE_SETTER(SafeDouble, VT_R8, dblVal)
AUTOCOM_SAFE_SETTER(SafeCurrency, VT_CY, cyVal)
//AUTOCOM_SAFE_SETTER(SafeBstr, VT_BSTR, bstrVal)

// CLEANUP -- SETTERS
// ------------------

#undef AUTOCOM_SAFE_MOVE
#undef AUTOCOM_SAFE_COPY
#undef AUTOCOM_SET_PRIMITIVE
#undef AUTOCOM_SET_MOVE
#undef AUTOCOM_SET_COPY
#undef AUTOCOM_SAFE_SETTER
#undef AUTOCOM_PRIMITIVE_SETTER
#undef AUTOCOM_CLASS_SETTER

// MACROS -- GETTERS
// -----------------


/** \brief Cast value to expected vartype in variant.
 */
#define AUTOCOM_CONVERT_TYPE(variant, vartype)                          \
    if (!(variant.vt & vartype)) {                                      \
        auto hr = VariantChangeType(&variant, &variant, 0, vartype);    \
        if (FAILED(hr)) {                                               \
            throw VariantGetValueError();                               \
        }                                                               \
    }                                                                   \


/** \brief Get value from variant.
 */
#define AUTOCOM_GET(type, vartype, field)                               \
                                                                        \
    void getVariant(VARIANT &variant,                                   \
        type *value)                                                    \
    {                                                                   \
        AUTOCOM_CONVERT_TYPE(variant, vartype)                          \
        *value = variant.field;                                         \
    }


/** \brief Get value using type-safe wrapper.
 */
#define AUTOCOM_SAFE_GET(safe, vartype, field)                          \
                                                                        \
    void getVariant(VARIANT &variant,                                   \
        safe &value)                                                    \
    {                                                                   \
        typedef typename safe::type Type;                               \
        AUTOCOM_CONVERT_TYPE(variant, vartype)                          \
        *Type(value) = variant.field;                                   \
    }


/** \brief Define a getter for type by value and by reference.
 *
 *  AUTOCOM_GETTER(BSTR, VT_BSTR, bstrVal)
 */
#define AUTOCOM_GETTER(type, vartype, field)                            \
    AUTOCOM_GET(type, vartype, field)                                   \
    AUTOCOM_GET(type*, vartype | VT_BYREF, p##field)


/** \brief Define a getter for type by value and by reference.
 *
 *  AUTOCOM_SAFE_GETTER(SafeBool, VT_BOOL, boolVal)
 */
#define AUTOCOM_SAFE_GETTER(safe, vartype, field)                       \
    AUTOCOM_SAFE_GET(safe##Ref, vartype, field)                         \
    AUTOCOM_SAFE_GET(safe##RefRef, vartype | VT_BYREF, p##field)



// FUNCTIONS -- GETTERS
// --------------------

/** \brief Get pointer to variant.
 */
void getVariant(VARIANT &variant,
    VARIANT **value)
{
    AUTOCOM_CONVERT_TYPE(variant, VT_VARIANT | VT_BYREF);
    *value = variant.pvarVal;
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
AUTOCOM_GETTER(CURRENCY, VT_CY, cyVal)
AUTOCOM_GETTER(BSTR, VT_BSTR, bstrVal)
AUTOCOM_GETTER(IUnknown*, VT_UNKNOWN, punkVal)
AUTOCOM_GETTER(IDispatch*, VT_DISPATCH, pdispVal)

// SAFE
AUTOCOM_SAFE_GETTER(SafeBool, VT_BOOL, boolVal)
AUTOCOM_SAFE_GETTER(SafeChar, VT_I1, cVal)
AUTOCOM_SAFE_GETTER(SafeUChar, VT_UI1, bVal)
AUTOCOM_SAFE_GETTER(SafeShort, VT_I2, iVal)
AUTOCOM_SAFE_GETTER(SafeUShort, VT_UI2, uiVal)
AUTOCOM_SAFE_GETTER(SafeInt, VT_INT, intVal)
AUTOCOM_SAFE_GETTER(SafeUInt, VT_UINT, uintVal)
AUTOCOM_SAFE_GETTER(SafeLong, VT_I4, lVal)
AUTOCOM_SAFE_GETTER(SafeULong, VT_UI4, ulVal)
AUTOCOM_SAFE_GETTER(SafeFloat, VT_R4, fltVal)
AUTOCOM_SAFE_GETTER(SafeDouble, VT_R8, dblVal)
AUTOCOM_SAFE_GETTER(SafeCurrency, VT_CY, cyVal)
//AUTOCOM_SAFE_GET(SafeBstrRef, VT_BSTR, bstrVal)

// CLEANUP -- GETTERS
// ------------------

#undef AUTOCOM_CONVERT_TYPE
#undef AUTOCOM_GET
#undef AUTOCOM_SAFE_GET
#undef AUTOCOM_GETTER
#undef AUTOCOM_SAFE_GETTER

// OBJECTS
// -------


/** \brief Null constructor.
 */
Variant::Variant()
{
    init();
}


/** \brief Destructor.
 */
Variant::~Variant()
{
    clear();
}


/** \brief Initialize variant.
 */
void Variant::init()
{
    VariantInit(this);
}


/** \brief Clear variant.
 */
void Variant::clear()
{
    VariantClear(this);
}


}   /* autocom */
