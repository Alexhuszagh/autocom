//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief Variant object and collection definitions.
 */

#include "autocom/safearray.hpp"
#include "autocom/variant.hpp"
#include "autocom/encoding/converters.hpp"


namespace autocom
{
// FUNCTIONS
// ---------


/** \brief Convert VARIANT data to new type.
 */
bool changeVariantType(VARIANT &variant,
    const VARTYPE vt)
{
    return VariantChangeType(&variant, &variant, 0, vt) == S_OK;
}

// MACROS
// ------

// PRIMITIVE

/** \brief Define a setter for a C primitive type.
 */
#define AUTOCOM_SET_PRIMITIVE(type, field)                              \
                                                                        \
    void set(VARIANT &variant,                                          \
        type const value)                                               \
    {                                                                   \
        variant.vt = VariantType<type>::vt;                             \
        variant.field = value;                                          \
    }

/** \brief Generalized dispparams setter overloaded for a primitive type.
 *
 *  AUTOCOM_PRIMITIVE_SETTER(CHAR, VT_I1, cVal)
 */
#define AUTOCOM_PRIMITIVE_SETTER(type, field)                           \
    AUTOCOM_SET_PRIMITIVE(type, field)                                  \
    AUTOCOM_SET_PRIMITIVE(type*, p##field)

// CLASS

/** \brief Define a setter for a custom type using move semantics.
 */
#define AUTOCOM_SET_MOVE(type, field)                                   \
                                                                        \
    void set(VARIANT &variant,                                          \
        type &&value)                                                   \
    {                                                                   \
        variant.vt = VariantType<type>::vt;                             \
        variant.field = std::move(value);                               \
    }


/** \brief Define a setter for a custom type using copy semantics.
 */
#define AUTOCOM_SET_COPY(type, field)                                   \
                                                                        \
    void set(VARIANT &variant,                                          \
        type const &value)                                              \
    {                                                                   \
        variant.vt = VariantType<type>::vt;                             \
        variant.field = value;                                          \
    }

/** \brief Generalized dispparams setter overloaded for a class values.
 *
 *  AUTOCOM_CLASS_VALUE_SETTER(CURRENCY, VT_CY, cyVal)
 */
#define AUTOCOM_CLASS_VALUE_SETTER(type, field)                         \
    AUTOCOM_SET_MOVE(type, field)                                       \
    AUTOCOM_SET_COPY(type, field)

/** \brief Generalized dispparams setter overloaded for a class values.
 *
 *  AUTOCOM_CLASS_POINTER_SETTER(CURRENCY, VT_CY, cyVal)
 */
#define AUTOCOM_CLASS_POINTER_SETTER(type, field)                       \
    AUTOCOM_SET_MOVE(type*, p##field)                                   \
    AUTOCOM_SET_COPY(type*, p##field)

/** \brief Generalized dispparams setter overloaded for a class types.
 *
 *  AUTOCOM_CLASS_SETTER(CURRENCY, VT_CY, cyVal)
 */
#define AUTOCOM_CLASS_SETTER(type, field)                               \
    AUTOCOM_CLASS_VALUE_SETTER(type, field)                             \
    AUTOCOM_CLASS_POINTER_SETTER(type, field)

// SAFE

/** \brief Type-safe class wrapper with move semantics.
 */
#define AUTOCOM_SAFE_MOVE(safe, field)                                  \
                                                                        \
    void set(VARIANT &variant,                                          \
        safe &&value)                                                   \
    {                                                                   \
        variant.vt = VariantType<safe>::vt;                             \
        variant.field = typename safe::type(std::move(value));          \
    }

/** \brief Type-safe class wrapper with copy semantics.
 */
#define AUTOCOM_SAFE_COPY(safe, field)                                  \
                                                                        \
    void set(VARIANT &variant,                                          \
        safe const &value)                                              \
    {                                                                   \
        variant.vt = VariantType<safe>::vt;                             \
        variant.field = typename safe::type(value);                     \
    }

/** \brief Type-safe wrapper for value setters.
 */
#define AUTOCOM_SAFE_VALUE_SETTER(type, field)                          \
    AUTOCOM_SAFE_MOVE(Put##type, field)                                 \
    AUTOCOM_SAFE_COPY(Put##type, field)

/** \brief Type-safe wrapper for pointer setters.
 */
#define AUTOCOM_SAFE_POINTER_SETTER(type, field)                        \
    AUTOCOM_SAFE_VALUE_SETTER(type##Ptr, p##field)

/** \brief Generalized type-safe wrappers for a given type.
 *
 *  Since the type is specialized for TypeWrapper, which provides
 *  a conversion-safe wrapper, any automation type can be passed.
 *
 *  AUTOCOM_SAFE_SETTER(Bool, boolVal, boolVal)
 */
#define AUTOCOM_SAFE_SETTER(type, field)                                \
    AUTOCOM_SAFE_VALUE_SETTER(type, field)                              \
    AUTOCOM_SAFE_POINTER_SETTER(type, field)


// FUNCTIONS -- SETTERS
// --------------------

/** \brief Set a null parameter.
 */
void set(VARIANT &variant,
    std::nullptr_t value)
{
    variant.vt = VariantType<std::nullptr_t>::vt;
}


/** \brief Set a null parameter.
 */
void set(VARIANT &variant,
    PutNull value)
{
    variant.vt = VT_NULL;
}


/** \brief Overload from character literals.
 */
void set(VARIANT &variant,
    const char *value)
{
    variant.vt = VT_BSTR;
    auto wide = WIDE(std::string(value));
    variant.bstrVal = SysAllocStringLen(wide.data(), wide.size());
}

/** \brief Overload from character literals.
 */
void set(VARIANT &variant,
    const wchar_t *value)
{
    variant.vt = VT_BSTR;
    variant.bstrVal = SysAllocStringLen(value, wcslen(value));
}


/** \brief Set a BSTR value from copy.
 */
void set(VARIANT &variant,
    BSTR &value)
{
    variant.vt = VariantType<BSTR>::vt;
    variant.bstrVal = value;
    value = nullptr;
}


/** \brief Set a BSTR value from move.
 */
void set(VARIANT &variant,
    BSTR &&value)
{
    variant.vt = VariantType<BSTR>::vt;
    variant.bstrVal = std::move(value);
}


/** \brief Set a pointer to a BSTR.
 */
void set(VARIANT &variant,
    BSTR *value)
{
    variant.vt = VariantType<BSTR*>::vt;
    variant.pbstrVal = value;
}


/** \brief Set a BSTR value from wrapper.
 */
void set(VARIANT &variant,
    Bstr &value)
{
    variant.vt = VariantType<Bstr>::vt;
    variant.bstrVal = value.data();
    value.data() = nullptr;
}

/** \brief Set a pointer to BSTR from wrapper.
 */
void set(VARIANT &variant,
    Bstr *value)
{
    variant.vt = VariantType<Bstr*>::vt;
    variant.pbstrVal = &value->data();
}


/** \brief Set a BSTR value from wrapper.
 */
void set(VARIANT &variant,
    PutBstr &&value)
{
    variant.vt = VariantType<PutBstr>::vt;
    variant.bstrVal = typename PutBstr::type(value);
}


/** \brief Set a BSTR value from wrapper.
 */
void set(VARIANT &variant,
    PutBstr &value)
{
    variant.vt = VariantType<PutBstr>::vt;
    variant.bstrVal = typename PutBstr::type(value);
    value = PutBstr(nullptr);
}


/** \brief Set a pointer to BSTR from wrapper.
 */
void set(VARIANT &variant,
    PutBstrPtr value)
{
    variant.vt = VariantType<PutBstrPtr>::vt;
    variant.pbstrVal = typename PutBstrPtr::type(value);
}


/** \brief Set a pointer to a SAFEARRAY pointer.
 */
void set(VARIANT &variant,
    SAFEARRAY *value)
{
    variant.vt = getSafeArrayType(value) | VT_ARRAY;
    variant.parray = value;
    value = nullptr;
}


/** \brief Set a pointer to a SAFEARRAY double pointer.
 */
void set(VARIANT &variant,
    SAFEARRAY **value)
{
    variant.vt = getSafeArrayType(*value) | VT_ARRAY | VT_BYREF;
    variant.pparray = value;
}


/** \brief Set a SAFEARRAY pointer-wrapper.
 */
void set(VARIANT &variant,
    PutSafeArray value)
{
    set(variant, typename PutSafeArray::type(value));
}


/** \brief Set a SAFEARRAY double pointer-wrapper.
 */
void set(VARIANT &variant,
    PutSafeArrayPtr value)
{
    set(variant, typename PutSafeArrayPtr::type(value));
}


// GENERIC
AUTOCOM_PRIMITIVE_SETTER(CHAR, cVal)
AUTOCOM_PRIMITIVE_SETTER(UCHAR, bVal)
AUTOCOM_PRIMITIVE_SETTER(SHORT, iVal)
AUTOCOM_PRIMITIVE_SETTER(USHORT, uiVal)
AUTOCOM_PRIMITIVE_SETTER(INT, intVal)
AUTOCOM_PRIMITIVE_SETTER(UINT, uintVal)
AUTOCOM_PRIMITIVE_SETTER(LONG, lVal)
AUTOCOM_PRIMITIVE_SETTER(ULONG, ulVal)
AUTOCOM_PRIMITIVE_SETTER(LONGLONG, llVal)
AUTOCOM_PRIMITIVE_SETTER(ULONGLONG, ullVal)
AUTOCOM_PRIMITIVE_SETTER(FLOAT, fltVal)
AUTOCOM_PRIMITIVE_SETTER(DOUBLE, dblVal)
AUTOCOM_CLASS_SETTER(CURRENCY, cyVal)
AUTOCOM_CLASS_POINTER_SETTER(DECIMAL, decVal)
AUTOCOM_CLASS_VALUE_SETTER(VARIANT*, pvarVal)
AUTOCOM_CLASS_VALUE_SETTER(Variant*, pvarVal)
AUTOCOM_CLASS_SETTER(IUnknown*, punkVal)
AUTOCOM_CLASS_SETTER(IDispatch*, pdispVal)

// SAFE
AUTOCOM_SAFE_SETTER(Bool, boolVal)
AUTOCOM_SAFE_SETTER(Char, cVal)
AUTOCOM_SAFE_SETTER(UChar, bVal)
AUTOCOM_SAFE_SETTER(Short, iVal)
AUTOCOM_SAFE_SETTER(UShort, uiVal)
AUTOCOM_SAFE_SETTER(Int, intVal)
AUTOCOM_SAFE_SETTER(UInt, uintVal)
AUTOCOM_SAFE_SETTER(Long, lVal)
AUTOCOM_SAFE_SETTER(ULong, ulVal)
AUTOCOM_SAFE_SETTER(Float, fltVal)
AUTOCOM_SAFE_SETTER(Double, dblVal)
AUTOCOM_SAFE_SETTER(LongLong, llVal)
AUTOCOM_SAFE_SETTER(ULongLong, ullVal)
AUTOCOM_SAFE_SETTER(Currency, cyVal)
AUTOCOM_SAFE_SETTER(Error, scode)
AUTOCOM_SAFE_SETTER(Date, date)
AUTOCOM_SAFE_SETTER(IUnknown, punkVal)
AUTOCOM_SAFE_SETTER(IDispatch, pdispVal)
AUTOCOM_SAFE_VALUE_SETTER(Variant, pvarVal)
AUTOCOM_SAFE_POINTER_SETTER(Decimal, decVal)

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
    if (TO_VARTYPE(variant.vt) != TO_VARTYPE(vartype)) {                \
        if (!changeVariantType(variant, vartype)) {                     \
            throw ComFunctionError("VariantChangeType");                \
        }                                                               \
    }

// GENERIC

/** \brief Get value from variant.
 */
#define AUTOCOM_GET(type, field)                                        \
                                                                        \
    void get(VARIANT &variant,                                          \
        type &value)                                                    \
    {                                                                   \
        AUTOCOM_CONVERT_TYPE(variant, VariantType<type>::vt)            \
        value = variant.field;                                          \
    }

/** \brief Define a getter for type by value.
 *
 *  AUTOCOM_VALUE_GETTER(BSTR, VT_BSTR, bstrVal)
 */
#define AUTOCOM_VALUE_GETTER(type, field)                               \
    AUTOCOM_GET(type, field)

/** \brief Define a getter for type by reference.
 *
 *  AUTOCOM_POINTER_GETTER(BSTR, VT_BSTR, bstrVal)
 */
#define AUTOCOM_POINTER_GETTER(type, field)                             \
    AUTOCOM_GET(type*, p##field)

/** \brief Define a getter for type by value and by reference.
 *
 *  AUTOCOM_GETTER(BSTR, VT_BSTR, bstrVal)
 */
#define AUTOCOM_GETTER(type, field)                                     \
    AUTOCOM_VALUE_GETTER(type, field)                                   \
    AUTOCOM_POINTER_GETTER(type, field)

// SAFE

/** \brief Get value using type-safe wrapper.
 */
#define AUTOCOM_SAFE_GET(safe, field)                                   \
                                                                        \
    void get(VARIANT &variant,                                          \
        safe value)                                                     \
    {                                                                   \
        auto &ref = typename safe::type(value);                         \
        AUTOCOM_CONVERT_TYPE(variant, VariantType<safe>::vt)            \
        ref = variant.field;                                            \
    }

/** \brief Define a getter for type by value.
 *
 *  AUTOCOM_SAFE_VALUE_GETTER(SafeBool, VT_BOOL, boolVal)
 */
#define AUTOCOM_SAFE_VALUE_GETTER(type, field)                          \
    AUTOCOM_SAFE_GET(Get##type, field)

/** \brief Define a getter for type by reference.
 *
 *  AUTOCOM_SAFE_POINTER_GETTER(SafeBool, VT_BOOL, boolVal)
 */
#define AUTOCOM_SAFE_POINTER_GETTER(type, field)                        \
    AUTOCOM_SAFE_VALUE_GETTER(type##Ptr, p##field)

/** \brief Define a getter for type by value and by reference.
 *
 *  AUTOCOM_SAFE_GETTER(SafeBool, VT_BOOL, boolVal)
 */
#define AUTOCOM_SAFE_GETTER(type, field)                                \
    AUTOCOM_SAFE_VALUE_GETTER(type, field)                              \
    AUTOCOM_SAFE_POINTER_GETTER(type, field)

// FUNCTIONS -- GETTERS
// --------------------


/** \brief Get BSTR value.
 */
void get(VARIANT &variant,
    BSTR &value)
{
    AUTOCOM_CONVERT_TYPE(variant, VariantType<Bstr>::vt);
    value = variant.bstrVal;
    variant.bstrVal = nullptr;
}


/** \brief Get BSTR reference.
 */
void get(VARIANT &variant,
    BSTR *&value)
{
    AUTOCOM_CONVERT_TYPE(variant, VariantType<Bstr>::vt);
    value = variant.pbstrVal;
}


/** \brief Get BSTR value in wrapper.
 */
void get(VARIANT &variant,
    Bstr &value)
{
    AUTOCOM_CONVERT_TYPE(variant, VariantType<Bstr>::vt);
    value.clear();
    value.data() = variant.bstrVal;
    variant.bstrVal = nullptr;
}


/** \brief Get BSTR value pointer in wrapper.
 */
void get(VARIANT &variant,
    Bstr *&value)
{
    AUTOCOM_CONVERT_TYPE(variant, VariantType<Bstr*>::vt);
    value->clear();
    value->data() = *variant.pbstrVal;
}


/** \brief Get BSTR value in wrapper.
 */
void get(VARIANT &variant,
    GetBstr value)
{
    AUTOCOM_CONVERT_TYPE(variant, VariantType<GetBstr>::vt);
    auto &ref = typename GetBstr::type(value);
    ref = variant.bstrVal;
    variant.bstrVal = nullptr;
}


/** \brief Get BSTR value pointer in wrapper.
 */
void get(VARIANT &variant,
    GetBstrPtr value)
{
    AUTOCOM_CONVERT_TYPE(variant, VariantType<Bstr*>::vt);
    auto &ref = typename GetBstrPtr::type(value);
    ref = variant.pbstrVal;
}


/** \brief Get VARIANT pointer.
 */
void get(VARIANT &variant,
    VARIANT *&value)
{
    AUTOCOM_CONVERT_TYPE(variant, VariantType<VARIANT*>::vt)
    value = variant.pvarVal;
    variant.pvarVal = nullptr;
}


/** \brief Get Variant value pointer in owning-wrapper.
 *
 *  This is **DEFINED** behavior in C++11 and above with the definition
 *  of standard layout types, which VARIANT and Variant statisfy, and
 *  **UNDEFINED** with C++03. The static_assert is to prevent compilation
 *  if reinterpret_cast could invoke undefined behavior.
 */
void get(VARIANT &variant,
    Variant *&value)
{
    static_assert(std::is_standard_layout<Variant>::value, "Variant is not standard layout");

    AUTOCOM_CONVERT_TYPE(variant, VariantType<Variant*>::vt)
    value = reinterpret_cast<Variant*>(variant.pvarVal);
    variant.pvarVal = nullptr;
}


/** \brief Get VARIANT pointer in wrapper.
 */
void get(VARIANT &variant,
    GetVariant value)
{
    get(variant, typename GetVariant::type(value));
}


/** \brief Get IUnknown by value.
 */
void get(VARIANT &variant,
    IUnknown *&value)
{
    AUTOCOM_CONVERT_TYPE(variant, VariantType<IUnknown*>::vt)
    value = variant.punkVal;
    variant.punkVal = nullptr;
}


/** \brief Get IUnknown by value in wrapper.
 */
void get(VARIANT &variant,
    GetIUnknown value)
{
    get(variant, typename GetIUnknown::type(value));
}


/** \brief Get IDispatch by value.
 */
void get(VARIANT &variant,
    IDispatch *&value)
{
    AUTOCOM_CONVERT_TYPE(variant, VariantType<IDispatch*>::vt)
    value = variant.pdispVal;
    variant.pdispVal = nullptr;
}


/** \brief Get IDispatch by value in wrapper.
 */
void get(VARIANT &variant,
    GetIDispatch value)
{
    get(variant, typename GetIDispatch::type(value));
}


/** \brief Get SAFEARRAY pointer.
 */
void get(VARIANT &variant,
    SAFEARRAY *&value)
{
    if (!(variant.vt & VT_ARRAY)) {
        throw std::invalid_argument("Unrecognized type, expected VT_ARRAY, got: " + std::to_string(variant.vt));
    }
    value = variant.parray;
    variant.parray = nullptr;
}


/** \brief Get SAFEARRAY double pointer.
 */
void get(VARIANT &variant,
    SAFEARRAY **&value)
{
    if (!(variant.vt & (VT_ARRAY | VT_BYREF))) {
        throw std::invalid_argument("Unrecognized type, expected VT_ARRAY | VT_BYREF, got: " + std::to_string(variant.vt));
    }
    value = variant.pparray;
}


/** \brief Get SAFEARRAY pointer-wrapper.
 */
void get(VARIANT &variant,
    GetSafeArray value)
{
    get(variant, typename GetSafeArray::type(value));
}

/** \brief Get SAFEARRAY double pointer-wrapper.
 */
void get(VARIANT &variant,
    GetSafeArrayPtr value)
{
    get(variant, typename GetSafeArrayPtr::type(value));
}


// GENERIC
AUTOCOM_GETTER(CHAR, cVal)
AUTOCOM_GETTER(UCHAR, bVal)
AUTOCOM_GETTER(SHORT, iVal)
AUTOCOM_GETTER(USHORT, uiVal)
AUTOCOM_GETTER(INT, intVal)
AUTOCOM_GETTER(UINT, uintVal)
AUTOCOM_GETTER(LONG, lVal)
AUTOCOM_GETTER(ULONG, ulVal)
AUTOCOM_GETTER(FLOAT, fltVal)
AUTOCOM_GETTER(DOUBLE, dblVal)
AUTOCOM_GETTER(LONGLONG, llVal)
AUTOCOM_GETTER(ULONGLONG, ullVal)
AUTOCOM_POINTER_GETTER(IUnknown*, punkVal)
AUTOCOM_POINTER_GETTER(IDispatch*, pdispVal)
AUTOCOM_GETTER(CURRENCY, cyVal)
AUTOCOM_POINTER_GETTER(DECIMAL, decVal)

// SAFE
AUTOCOM_SAFE_GETTER(Bool, boolVal)
AUTOCOM_SAFE_GETTER(Char, cVal)
AUTOCOM_SAFE_GETTER(UChar, bVal)
AUTOCOM_SAFE_GETTER(Short, iVal)
AUTOCOM_SAFE_GETTER(UShort, uiVal)
AUTOCOM_SAFE_GETTER(Int, intVal)
AUTOCOM_SAFE_GETTER(UInt, uintVal)
AUTOCOM_SAFE_GETTER(Long, lVal)
AUTOCOM_SAFE_GETTER(ULong, ulVal)
AUTOCOM_SAFE_GETTER(Float, fltVal)
AUTOCOM_SAFE_GETTER(LongLong, llVal)
AUTOCOM_SAFE_GETTER(ULongLong, ullVal)
AUTOCOM_SAFE_GETTER(Double, dblVal)
AUTOCOM_SAFE_GETTER(Currency, cyVal)
AUTOCOM_SAFE_GETTER(Error, scode)
AUTOCOM_SAFE_GETTER(Date, date)
AUTOCOM_SAFE_POINTER_GETTER(IUnknown, punkVal)
AUTOCOM_SAFE_POINTER_GETTER(IDispatch, pdispVal)
AUTOCOM_SAFE_POINTER_GETTER(Decimal, decVal)

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


/** \brief Copy constructor.
 */
Variant::Variant(const Variant &other)
{
    VariantCopy(this, const_cast<Variant*>(&other));
}


/** \brief Copy assignment operator.
 */
Variant & Variant::operator=(const Variant &other)
{
    VariantCopy(this, const_cast<Variant*>(&other));
    return *this;
}


/** \brief Move constructor.
 */
Variant::Variant(Variant &&other):
    VARIANT(static_cast<VARIANT&&>(other))
{
    other.vt = VT_EMPTY;
}


/** \brief Move assignment operator.
 */
Variant & Variant::operator=(Variant &&other)
{
    VARIANT::operator=(static_cast<VARIANT&&>(other));
    other.vt = VT_EMPTY;
    return *this;
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


/** \brief Coerce variant type to specified type.
 */
bool Variant::changeType(const VARTYPE vt)
{
    return changeVariantType(*this, vt);
}


/** \brief Reset variant with no value.
 */
void Variant::reset()
{
    clear();
}

}   /* autocom */
