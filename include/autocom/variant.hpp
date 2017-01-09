//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief Variant object and collection definitions.
 */

#pragma once

#include "bstr.hpp"
#include "util/type.hpp"
#include "util/define.hpp"
#include "util/variadic.hpp"

#include <oaidl.h>

#include <vector>


namespace autocom
{
// FORWARD
// -------

struct Variant;

// SFINAE
// ------

template <typename T>
using IsVariant = std::is_convertible<T&, VARIANT&>;

template <typename T>
constexpr bool IsVariantV = IsVariant<T>::value;

// FUNCTIONS
// ---------


/** \brief Convert VARIANT data to new type.
 *
 *  \return             Type coercion was successful
 */
bool changeVariantType(VARIANT &variant,
    const VARTYPE vt);

/** \brief Static cast type to VARTYPE.
 *
 *  Required for MinGW, and other compilers to prevent integer promotion.
 */
template <typename T>
VARTYPE TO_VARTYPE(const T t)
{
    return static_cast<VARTYPE>(t);
}

// MACROS -- SETTERS
// -----------------

// PRIMITIVE

/** \brief Define a setter for a C primitive type.
 */
#define AUTOCOM_SET_PRIMITIVE(type)                                     \
    void set(VARIANT &variant,                                          \
        type const value)

/** \brief Generalized dispparams setter overloaded for a primitive type.
 *
 *  AUTOCOM_PRIMITIVE_SETTER(CHAR, VT_I1, cVal)
 */
#define AUTOCOM_PRIMITIVE_SETTER(type)                                  \
    AUTOCOM_SET_PRIMITIVE(type);                                        \
    AUTOCOM_SET_PRIMITIVE(type*)

// CLASS

/** \brief Define a setter for a custom type using move semantics.
 */
#define AUTOCOM_SET_MOVE(type)                                          \
    void set(VARIANT &variant,                                          \
        type &&value)

/** \brief Define a setter for a custom type using copy semantics.
 */
#define AUTOCOM_SET_COPY(type)                                          \
    void set(VARIANT &variant,                                          \
        type const &value)

/** \brief Generalized dispparams setter overloaded for a class values.
 *
 *  AUTOCOM_CLASS_VALUE_SETTER(CURRENCY, VT_CY, cyVal)
 */
#define AUTOCOM_CLASS_VALUE_SETTER(type)                                \
    AUTOCOM_SET_MOVE(type);                                             \
    AUTOCOM_SET_COPY(type)

/** \brief Generalized dispparams setter overloaded for a class values.
 *
 *  AUTOCOM_CLASS_POINTER_SETTER(CURRENCY, VT_CY, cyVal)
 */
#define AUTOCOM_CLASS_POINTER_SETTER(type)                              \
    AUTOCOM_SET_MOVE(type*);                                            \
    AUTOCOM_SET_COPY(type*)

/** \brief Generalized dispparams setter overloaded for a class types.
 *
 *  AUTOCOM_CLASS_SETTER(CURRENCY, VT_CY, cyVal)
 */
#define AUTOCOM_CLASS_SETTER(type)                                      \
    AUTOCOM_CLASS_VALUE_SETTER(type);                                   \
    AUTOCOM_CLASS_POINTER_SETTER(type)

// SAFE

/** \brief Type-safe class wrapper with move semantics.
 */
#define AUTOCOM_SAFE_MOVE(safe)                                         \
    void set(VARIANT &variant,                                          \
        safe &&value)

/** \brief Type-safe class wrapper with copy semantics.
 */
#define AUTOCOM_SAFE_COPY(safe)                                         \
    void set(VARIANT &variant,                                          \
        safe const &value)

/** \brief Type-safe wrapper for value setters.
 */
#define AUTOCOM_SAFE_VALUE_SETTER(type)                                 \
    AUTOCOM_SAFE_MOVE(Put##type);                                       \
    AUTOCOM_SAFE_COPY(Put##type);                                       \
    AUTOCOM_SAFE_MOVE(Get##type);                                       \
    AUTOCOM_SAFE_COPY(Get##type)

/** \brief Type-safe wrapper for pointer setters.
 */
#define AUTOCOM_SAFE_POINTER_SETTER(type)                               \
    AUTOCOM_SAFE_VALUE_SETTER(type##Ptr)

/** \brief Generalized type-safe wrappers for a given type.
 *
 *  Since the type is specialized for TypeWrapper, which provides
 *  a conversion-safe wrapper, any automation type can be passed.
 *
 *  AUTOCOM_SAFE_SETTER(Bool, boolVal, boolVal)
 */
#define AUTOCOM_SAFE_SETTER(type)                                       \
    AUTOCOM_SAFE_VALUE_SETTER(type);                                    \
    AUTOCOM_SAFE_POINTER_SETTER(type)

// FUNCTIONS -- SETTERS
// --------------------


/** \brief Set a null parameter.
 */
void set(VARIANT &variant,
    std::nullptr_t value);

/** \brief Set a null parameter.
 */
void set(VARIANT &variant,
    PutNull value);

/** \brief Set a null parameter.
 */
void set(VARIANT &variant,
    GetNull value);

/** \brief Overload from character literals.
 */
void set(VARIANT &variant,
    const char *value);

/** \brief Overload from character literals.
 */
void set(VARIANT &variant,
    const wchar_t *value);

/** \brief Set a BSTR value from copy.
 */
void set(VARIANT &variant,
    BSTR &value);

/** \brief Set a BSTR value from move.
 */
void set(VARIANT &variant,
    BSTR &&value);

/** \brief Set a pointer to a BSTR.
 */
void set(VARIANT &variant,
    BSTR *value);

/** \brief Set a BSTR value from wrapper.
 */
void set(VARIANT &variant,
    Bstr &value);

/** \brief Set a BSTR value from wrapper.
 */
void set(VARIANT &variant,
    Bstr &&value);

/** \brief Set a pointer to BSTR from wrapper.
 */
void set(VARIANT &variant,
    Bstr *value);

/** \brief Set a BSTR value from wrapper.
 */
void set(VARIANT &variant,
    PutBstr &&value);

/** \brief Set a BSTR value from wrapper.
 */
void set(VARIANT &variant,
    PutBstr &value);

/** \brief Set a pointer to BSTR from wrapper.
 */
void set(VARIANT &variant,
    PutBstrPtr value);

/** \brief Set a BSTR value from wrapper.
 */
void set(VARIANT &variant,
    GetBstr &&value);

/** \brief Set a BSTR value from wrapper.
 */
void set(VARIANT &variant,
    GetBstr &value);

/** \brief Set a pointer to BSTR from wrapper.
 */
void set(VARIANT &variant,
    GetBstrPtr value);

/** \brief Set a pointer to a SAFEARRAY pointer.
 */
void set(VARIANT &variant,
    SAFEARRAY *value);

/** \brief Set a pointer to a SAFEARRAY double pointer.
 */
void set(VARIANT &variant,
    SAFEARRAY **value);

/** \brief Set a SAFEARRAY pointer-wrapper.
 */
void set(VARIANT &variant,
    PutSafeArray value);

/** \brief Set a SAFEARRAY double pointer-wrapper.
 */
void set(VARIANT &variant,
    PutSafeArrayPtr value);

/** \brief Set a SAFEARRAY pointer-wrapper.
 */
void set(VARIANT &variant,
    GetSafeArray value);

/** \brief Set a SAFEARRAY double pointer-wrapper.
 */
void set(VARIANT &variant,
    GetSafeArrayPtr value);


/** \brief Set SafeArray pointer.
 */
template <typename T>
void set(VARIANT &variant,
    SafeArray<T> &value)
{
    variant.vt = VariantType<T>::vt | VT_ARRAY;
    variant.parray = value.array;
    value.array = nullptr;
}


/** \brief Set SafeArray pointer from move.
 */
template <typename T>
void set(VARIANT &variant,
    SafeArray<T> &&value)
{
    variant.vt = VariantType<T>::vt | VT_ARRAY;
    variant.parray = value.array;
    value.array = nullptr;
}


/** \brief Set SafeArray double pointer.
 */
template <typename T>
void set(VARIANT &variant,
    SafeArray<T> *value)
{
    variant.vt = VariantType<T>::vt | VT_ARRAY | VT_BYREF;
    variant.pparray = &value->array;
}


// GENERIC
AUTOCOM_SET_PRIMITIVE(bool);
AUTOCOM_PRIMITIVE_SETTER(CHAR);
AUTOCOM_PRIMITIVE_SETTER(UCHAR);
AUTOCOM_PRIMITIVE_SETTER(SHORT);
AUTOCOM_PRIMITIVE_SETTER(USHORT);
AUTOCOM_PRIMITIVE_SETTER(INT);
AUTOCOM_PRIMITIVE_SETTER(UINT);
AUTOCOM_PRIMITIVE_SETTER(LONG);
AUTOCOM_PRIMITIVE_SETTER(ULONG);
AUTOCOM_PRIMITIVE_SETTER(LONGLONG);
AUTOCOM_PRIMITIVE_SETTER(ULONGLONG);
AUTOCOM_PRIMITIVE_SETTER(FLOAT);
AUTOCOM_PRIMITIVE_SETTER(DOUBLE);
AUTOCOM_CLASS_SETTER(CURRENCY);
AUTOCOM_CLASS_POINTER_SETTER(DECIMAL);
AUTOCOM_CLASS_VALUE_SETTER(VARIANT*);
AUTOCOM_CLASS_VALUE_SETTER(Variant*);
AUTOCOM_CLASS_SETTER(IUnknown*);
AUTOCOM_CLASS_SETTER(IDispatch*);

// SAFE
AUTOCOM_SAFE_SETTER(Bool);
AUTOCOM_SAFE_SETTER(Char);
AUTOCOM_SAFE_SETTER(UChar);
AUTOCOM_SAFE_SETTER(Short);
AUTOCOM_SAFE_SETTER(UShort);
AUTOCOM_SAFE_SETTER(Int);
AUTOCOM_SAFE_SETTER(UInt);
AUTOCOM_SAFE_SETTER(Long);
AUTOCOM_SAFE_SETTER(ULong);
AUTOCOM_SAFE_SETTER(Float);
AUTOCOM_SAFE_SETTER(Double);
AUTOCOM_SAFE_SETTER(LongLong);
AUTOCOM_SAFE_SETTER(ULongLong);
AUTOCOM_SAFE_SETTER(Currency);
AUTOCOM_SAFE_SETTER(Error);
AUTOCOM_SAFE_SETTER(Date);
AUTOCOM_SAFE_SETTER(IUnknown);
AUTOCOM_SAFE_SETTER(IDispatch);
AUTOCOM_SAFE_VALUE_SETTER(Variant);
AUTOCOM_SAFE_POINTER_SETTER(Decimal);

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
#define AUTOCOM_GET(type)                                               \
    void get(VARIANT &variant,                                          \
        type &value)

/** \brief Define a getter for type by value.
 *
 *  AUTOCOM_VALUE_GETTER(BSTR, VT_BSTR, bstrVal)
 */
#define AUTOCOM_VALUE_GETTER(type)                                      \
    AUTOCOM_GET(type)

/** \brief Define a getter for type by reference.
 *
 *  AUTOCOM_POINTER_GETTER(BSTR, VT_BSTR, bstrVal)
 */
#define AUTOCOM_POINTER_GETTER(type)                                    \
    AUTOCOM_GET(type*)

/** \brief Define a getter for type by value and by reference.
 *
 *  AUTOCOM_GETTER(BSTR, VT_BSTR, bstrVal)
 */
#define AUTOCOM_GETTER(type)                                            \
    AUTOCOM_VALUE_GETTER(type);                                         \
    AUTOCOM_POINTER_GETTER(type)

// SAFE

/** \brief Get value using type-safe wrapper.
 */
#define AUTOCOM_SAFE_GET(safe)                                          \
    void get(VARIANT &variant,                                          \
        safe value)

/** \brief Define a getter for type by value.
 *
 *  AUTOCOM_SAFE_VALUE_GETTER(SafeBool, VT_BOOL, boolVal)
 */
#define AUTOCOM_SAFE_VALUE_GETTER(type)                                 \
    AUTOCOM_SAFE_GET(Get##type)

/** \brief Define a getter for type by reference.
 *
 *  AUTOCOM_SAFE_POINTER_GETTER(SafeBool, VT_BOOL, boolVal)
 */
#define AUTOCOM_SAFE_POINTER_GETTER(type)                               \
    AUTOCOM_SAFE_VALUE_GETTER(type##Ptr)

/** \brief Define a getter for type by value and by reference.
 *
 *  AUTOCOM_SAFE_GETTER(SafeBool, VT_BOOL, boolVal)
 */
#define AUTOCOM_SAFE_GETTER(type)                                       \
    AUTOCOM_SAFE_VALUE_GETTER(type);                                    \
    AUTOCOM_SAFE_POINTER_GETTER(type)

// FUNCTIONS -- GETTERS
// --------------------

/** \brief Get BSTR value.
 */
void get(VARIANT &variant,
    BSTR &value);

/** \brief Get BSTR reference.
 */
void get(VARIANT &variant,
    BSTR *&value);

/** \brief Get BSTR value in wrapper.
 */
void get(VARIANT &variant,
    Bstr &value);

/** \brief Get BSTR value pointer in wrapper.
 */
void get(VARIANT &variant,
    Bstr *&value);

/** \brief Get BSTR value in wrapper.
 */
void get(VARIANT &variant,
    GetBstr value);

/** \brief Get BSTR value pointer in wrapper.
 */
void get(VARIANT &variant,
    GetBstrPtr value);

/** \brief Get VARIANT pointer.
 */
void get(VARIANT &variant,
    VARIANT *&value);

/** \brief Get Variant value pointer in owning-wrapper.
 */
void get(VARIANT &variant,
    Variant *&value);

/** \brief Get VARIANT pointer in wrapper.
 */
void get(VARIANT &variant,
    GetVariant value);

/** \brief Get IUnknown by value.
 */
void get(VARIANT &variant,
    IUnknown *&value);

/** \brief Get IUnknown by value in wrapper.
 */
void get(VARIANT &variant,
    GetIUnknown value);

/** \brief Get IDispatch by value.
 */
void get(VARIANT &variant,
    IDispatch *&value);

/** \brief Get IDispatch by value in wrapper.
 */
void get(VARIANT &variant,
    GetIDispatch value);

/** \brief Get SAFEARRAY pointer.
 */
void get(VARIANT &variant,
    SAFEARRAY *&value);

/** \brief Get SAFEARRAY double pointer.
 */
void get(VARIANT &variant,
    SAFEARRAY **&value);

/** \brief Get SAFEARRAY pointer-wrapper.
 */
void get(VARIANT &variant,
    GetSafeArray value);

/** \brief Get SAFEARRAY double pointer-wrapper.
 */
void get(VARIANT &variant,
    GetSafeArrayPtr value);


/** \brief Get SafeArray pointer.
 */
template <typename T>
void get(VARIANT &variant,
    SafeArray<T> &value)
{
    if (!(variant.vt & VariantType<T, true>::vt)) {
        throw std::invalid_argument("SafeArray types do not match.");
    }
    value.array = variant.parray;
    variant.parray = nullptr;
}


/** \brief Get SafeArray double pointer.
 */
template <typename T>
void get(VARIANT &variant,
    SafeArray<T> **&value)
{
    if (!(variant.vt & VariantType<T, true>::vt)) {
        throw std::invalid_argument("SafeArray types do not match.");
    }
    if (variant.pparray) {
        value->array = *variant.pparray;
    }
}


// GENERIC
AUTOCOM_GETTER(CHAR);
AUTOCOM_GETTER(UCHAR);
AUTOCOM_GETTER(SHORT);
AUTOCOM_GETTER(USHORT);
AUTOCOM_GETTER(INT);
AUTOCOM_GETTER(UINT);
AUTOCOM_GETTER(LONG);
AUTOCOM_GETTER(ULONG);
AUTOCOM_GETTER(FLOAT);
AUTOCOM_GETTER(DOUBLE);
AUTOCOM_GETTER(LONGLONG);
AUTOCOM_GETTER(ULONGLONG);
AUTOCOM_GETTER(CURRENCY);
AUTOCOM_POINTER_GETTER(IUnknown*);
AUTOCOM_POINTER_GETTER(IDispatch*);
AUTOCOM_POINTER_GETTER(DECIMAL);

// SAFE
AUTOCOM_SAFE_GETTER(Bool);
AUTOCOM_SAFE_GETTER(Char);
AUTOCOM_SAFE_GETTER(UChar);
AUTOCOM_SAFE_GETTER(Short);
AUTOCOM_SAFE_GETTER(UShort);
AUTOCOM_SAFE_GETTER(Int);
AUTOCOM_SAFE_GETTER(UInt);
AUTOCOM_SAFE_GETTER(Long);
AUTOCOM_SAFE_GETTER(ULong);
AUTOCOM_SAFE_GETTER(Float);
AUTOCOM_SAFE_GETTER(LongLong);
AUTOCOM_SAFE_GETTER(ULongLong);
AUTOCOM_SAFE_GETTER(Double);
AUTOCOM_SAFE_GETTER(Currency);
AUTOCOM_SAFE_GETTER(Error);
AUTOCOM_SAFE_GETTER(Date);
AUTOCOM_SAFE_POINTER_GETTER(IUnknown);
AUTOCOM_SAFE_POINTER_GETTER(IDispatch);
AUTOCOM_SAFE_POINTER_GETTER(Decimal);

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

    Variant(const Variant &other);
    Variant & operator=(const Variant &other);
    Variant(Variant &&other);
    Variant & operator=(Variant &&other);

    /** \brief Initalize and set variant value.
     *
     *  \warning For some reason, MSVC raises C2244 if the function
     *  declaration and implementation are separated.
     */
    template <typename T>
    Variant(T &&t,
        typename std::enable_if<!IsVariantV<T>, void>::type* = 0)
    {
        set(AUTOCOM_FWD(t));
    }

    // MODIFIERS
    void init();
    void clear();
    bool changeType(const VARTYPE vt);
    void reset();

    template <typename T>
    void set(T &&t);

    template <typename T>
    void reset(T &&t);

    template <typename T>
    void get(T &&t);
};


// DOWNCASTING
// -----------

static_assert(sizeof(Variant) == sizeof(VARIANT), "sizeof(Variant) != sizeof(VARIANT), cannot safely downcast");

// IMPLEMENTATION
// --------------


/** \brief Set value in variant.
 */
template <typename T>
void Variant::set(T &&t)
{
    clear();
    autocom::set(*this, AUTOCOM_FWD(t));
}


/** \brief Reset variant and set value.
 */
template <typename T>
void Variant::reset(T &&t)
{
    clear();
    set(AUTOCOM_FWD(t));
}


/** \brief Get value from variant.
 */
template <typename T>
void Variant::get(T &&t)
{
    autocom::get(*this, AUTOCOM_FWD(t));
}

// TYPES
// -----

typedef std::vector<Variant> VariantList;

}   /* autocom */
