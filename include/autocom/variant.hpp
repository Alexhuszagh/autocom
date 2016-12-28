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

#ifdef HAVE_PROPSYS
    /** \brief Convert PROPVARIANT data to new type.
     *
     *  \return             Type coercion was successful
     */
    bool changeVariantType(PROPVARIANT &variant,
        const VARTYPE vt);
#endif          // HAVE_PROPSYS

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
#define AUTOCOM_SAFE_VALUE_SETTER(safe, vartype, field)                 \
    AUTOCOM_SAFE_MOVE(safe, vartype, field)                             \
    AUTOCOM_SAFE_COPY(safe, vartype, field)

/** \brief Type-safe wrapper for pointer setters.
 */
#define AUTOCOM_SAFE_POINTER_SETTER(safe, vartype, field)               \
    AUTOCOM_SAFE_MOVE(safe##Ptr, vartype | VT_BYREF, p##field)          \
    AUTOCOM_SAFE_COPY(safe##Ptr, vartype | VT_BYREF, p##field)

/** \brief Generalized type-safe wrappers for a given type.
 *
 *  Since the type is specialized for TypeWrapper, which provides
 *  a conversion-safe wrapper, any automation type can be passed.
 *
 *  AUTOCOM_SAFE_SETTER(BOOL, boolVal, boolVal)
 */
#define AUTOCOM_SAFE_SETTER(safe, vartype, field)                       \
    AUTOCOM_SAFE_VALUE_SETTER(safe, vartype, field)                     \
    AUTOCOM_SAFE_POINTER_SETTER(safe, vartype, field)

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
    SafeNull value)
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
#ifdef HAVE_PROPSYS
    AUTOCOM_CLASS_SETTER(FILETIME, VT_FILETIME, filetime)
    AUTOCOM_CLASS_SETTER(CLSID, VT_CLSID, puuid)
    AUTOCOM_CLASS_SETTER(CLIPDATA, VT_CF, pclipdata)
    AUTOCOM_CLASS_VALUE_SETTER(IStream, VT_STREAM, pStream)
    AUTOCOM_CLASS_VALUE_SETTER(IStorage, VT_STORAGE, pStorage)
    AUTOCOM_CLASS_VALUE_SETTER(LPVERSIONEDSTREAM, VT_VERSIONED_STREAM, pVersionedStream)
#endif      // HAVE_PROPSYS

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
AUTOCOM_SAFE_SETTER(SafeLongLong, VT_I8, llVal)
AUTOCOM_SAFE_SETTER(SafeULongLong, VT_UI8, ullVal)
AUTOCOM_SAFE_SETTER(SafeBstr, VT_BSTR, bstrVal)
AUTOCOM_SAFE_SETTER(SafeCurrency, VT_CY, cyVal)
AUTOCOM_SAFE_SETTER(SafeError, VT_ERROR, scode)
AUTOCOM_SAFE_SETTER(SafeDate, VT_DATE, date)
AUTOCOM_SAFE_SETTER(SafeIUnknown, VT_UNKNOWN, punkVal)
AUTOCOM_SAFE_SETTER(SafeIDispatch, VT_DISPATCH, pdispVal)
AUTOCOM_SAFE_POINTER_SETTER(SafeDecimal, VT_DECIMAL, decVal)
// VT_VECTOR
// VT_ARRAY
#ifdef HAVE_PROPSYS
    AUTOCOM_SAFE_SETTER(SafeLargeInteger, VT_I8, hVal)
    AUTOCOM_SAFE_SETTER(SafeULargeInteger, VT_UI8, uhVal)
    AUTOCOM_SAFE_SETTER(SafeFiletime, VT_FILETIME, filetime)
    AUTOCOM_SAFE_SETTER(SafeClsid, VT_CLSID, puuid)
    AUTOCOM_SAFE_SETTER(SafeGuid, VT_CLSID, puuid)
    AUTOCOM_SAFE_SETTER(SafeClipData, VT_CF, pclipdata)
    AUTOCOM_SAFE_VALUE_SETTER(SafeIStream, VT_STREAM, pStream)
    AUTOCOM_SAFE_VALUE_SETTER(SafeIStreamObject, VT_STREAMED_OBJECT, pStream)
    AUTOCOM_SAFE_VALUE_SETTER(SafeIStorage, VT_STORAGE, pStorage)
    AUTOCOM_SAFE_VALUE_SETTER(SafeIStorageObject, VT_STORED_OBJECT, pStorage)
    AUTOCOM_SAFE_VALUE_SETTER(SafeLpVersionedStream, VT_VERSIONED_STREAM, pVersionedStream)
    AUTOCOM_SAFE_VALUE_SETTER(SafeBlob, VT_BLOB, blob)
    AUTOCOM_SAFE_VALUE_SETTER(SafeBlobObject, VT_BLOBOBJECT, blob)
    AUTOCOM_SAFE_VALUE_SETTER(SafeLpstr, VT_LPSTR, pszVal)
    AUTOCOM_SAFE_VALUE_SETTER(SafeLpwstr, VT_LPWSTR, pwszVal)
#endif      // HAVE_PROPSYS

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
            throw VariantGetValueError();                               \
        }                                                               \
    }                                                                   \


/** \brief Get value from variant.
 */
#define AUTOCOM_GET(type, vartype, field)                               \
                                                                        \
    template <typename VariantType>                                     \
    void getVariant(VariantType &variant,                               \
        type *value)                                                    \
    {                                                                   \
        AUTOCOM_CONVERT_TYPE(variant, vartype)                          \
        *value = variant.field;                                         \
    }


/** \brief Get value using type-safe wrapper.
 */
#define AUTOCOM_SAFE_GET(safe, vartype, field)                          \
                                                                        \
    template <typename VariantType>                                     \
    void getVariant(VariantType &variant,                               \
        safe &value)                                                    \
    {                                                                   \
        typedef typename safe::type Type;                               \
        AUTOCOM_CONVERT_TYPE(variant, vartype)                          \
        *Type(value) = variant.field;                                   \
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
#define AUTOCOM_SAFE_VALUE_GETTER(safe, vartype, field)                 \
    AUTOCOM_SAFE_GET(safe##Ptr, vartype, field)

/** \brief Define a getter for type by reference.
 *
 *  AUTOCOM_SAFE_POINTER_GETTER(SafeBool, VT_BOOL, boolVal)
 */
#define AUTOCOM_SAFE_POINTER_GETTER(safe, vartype, field)               \
    AUTOCOM_SAFE_GET(safe##DblPtr, vartype | VT_BYREF, p##field)

/** \brief Define a getter for type by value and by reference.
 *
 *  AUTOCOM_SAFE_GETTER(SafeBool, VT_BOOL, boolVal)
 */
#define AUTOCOM_SAFE_GETTER(safe, vartype, field)                       \
    AUTOCOM_SAFE_VALUE_GETTER(safe, vartype, field)                     \
    AUTOCOM_SAFE_POINTER_GETTER(safe, vartype, field)

// FUNCTIONS -- GETTERS
// --------------------


/** \brief Get pointer to variant.
 */
template <typename VariantType>
void getVariant(VariantType &variant,
    VARIANT **value)
{
    AUTOCOM_CONVERT_TYPE(variant, VT_VARIANT | VT_BYREF);
    *value = variant.pvarVal;
}


/** \brief Get BSTR value in wrapper.
 */
template <typename VariantType>
void getVariant(VariantType &variant,
    Bstr *value)
{
    AUTOCOM_CONVERT_TYPE(variant, VT_BSTR);
    value->clear();
    value->string = variant.bstrVal;
}


/** \brief Get BSTR value pointer in wrapper.
 */
template <typename VariantType>
void getVariant(VariantType &variant,
    Bstr **value)
{
    AUTOCOM_CONVERT_TYPE(variant, VT_BSTR | VT_BYREF);
    (*value)->clear();
    (*value)->string = *variant.pbstrVal;
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
#ifdef HAVE_PROPSYS
    AUTOCOM_GETTER(FILETIME, VT_FILETIME, filetime)
    AUTOCOM_GETTER(CLSID, VT_CLSID, puuid)
    AUTOCOM_GETTER(CLIPDATA, VT_CF, pclipdata)
    AUTOCOM_VALUE_GETTER(IStream, VT_STREAM, pStream)
    AUTOCOM_VALUE_GETTER(IStorage, VT_STORAGE, pStorage)
    AUTOCOM_VALUE_GETTER(LPVERSIONEDSTREAM, VT_VERSIONED_STREAM, pVersionedStream)
#endif      // HAVE_PROPSYS

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
AUTOCOM_SAFE_GETTER(SafeLongLong, VT_I8, llVal)
AUTOCOM_SAFE_GETTER(SafeULongLong, VT_UI8, ullVal)
AUTOCOM_SAFE_GETTER(SafeDouble, VT_R8, dblVal)
AUTOCOM_SAFE_GETTER(SafeBstr, VT_BSTR, bstrVal)
AUTOCOM_SAFE_GETTER(SafeCurrency, VT_CY, cyVal)
AUTOCOM_SAFE_GETTER(SafeIUnknown, VT_UNKNOWN, punkVal)
AUTOCOM_SAFE_GETTER(SafeIDispatch, VT_DISPATCH, pdispVal)
AUTOCOM_SAFE_POINTER_GETTER(SafeDecimal, VT_DECIMAL, decVal)
// VT_VECTOR
// VT_ARRAY
#ifdef HAVE_PROPSYS
    AUTOCOM_SAFE_GETTER(SafeLargeInteger, VT_I8, hVal)
    AUTOCOM_SAFE_GETTER(SafeULargeInteger, VT_UI8, uhVal)
    AUTOCOM_SAFE_GETTER(SafeFiletime, VT_FILETIME, filetime)
    AUTOCOM_SAFE_GETTER(SafeClsid, VT_CLSID, puuid)
    AUTOCOM_SAFE_GETTER(SafeGuid, VT_CLSID, puuid)
    AUTOCOM_SAFE_GETTER(SafeClipData, VT_CF, pclipdata)
    AUTOCOM_SAFE_VALUE_GETTER(SafeIStream, VT_STREAM, pStream)
    AUTOCOM_SAFE_VALUE_GETTER(SafeIStreamObject, VT_STREAMED_OBJECT, pStream)
    AUTOCOM_SAFE_VALUE_GETTER(SafeIStorage, VT_STORAGE, pStorage)
    AUTOCOM_SAFE_VALUE_GETTER(SafeIStorageObject, VT_STORED_OBJECT, pStorage)
    AUTOCOM_SAFE_VALUE_GETTER(SafeLpVersionedStream, VT_VERSIONED_STREAM, pVersionedStream)
    AUTOCOM_SAFE_VALUE_GETTER(SafeBlob, VT_BLOB, blob)
    AUTOCOM_SAFE_VALUE_GETTER(SafeBlobObject, VT_BLOBOBJECT, blob)
    AUTOCOM_SAFE_VALUE_GETTER(SafeLpstr, VT_LPSTR, pszVal)
    AUTOCOM_SAFE_VALUE_GETTER(SafeLpwstr, VT_LPWSTR, pwszVal)
#endif      // HAVE_PROPSYS

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


#ifdef HAVE_PROPSYS

    /** \brief C++ wrapper for PROPVARIANT, a property VARIANT.
     */
    struct PropVariant: public PROPVARIANT
    {
        PropVariant();
        ~PropVariant();

        void init();
        void clear();
        bool changeType(const VARTYPE vt);

        template <typename T>
        void set(T &&t);

        template <typename T>
        void get(T &&t);
    };

#endif          // HAVE_PROPSYS


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
    getVariant(*this, AUTOCOM_FWD(t));
}


#ifdef HAVE_PROPSYS

    /** \brief Set value in property variant.
     */
    template <typename T>
    void PropVariant::set(T &&t)
    {
        setVariant(*this, AUTOCOM_FWD(t));
    }


    /** \brief Get value from property variant.
     */
    template <typename T>
    void PropVariant::get(T &&t)
    {
        getVariant(*this, AUTOCOM_FWD(t));
    }

#endif          // HAVE_PROPSYS


// TYPES
// -----

typedef std::vector<Variant> VariantList;

}   /* autocom */
