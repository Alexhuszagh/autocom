//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCom
 *  \brief Variant object and collection definitions.
 */

#pragma once

#include "util.hpp"

#include <oaidl.h>
#include <windows.h>
#include <wtypes.h>

#include <initializer_list>
#include <vector>


namespace autocom
{
// MACROS -- SETTERS
// -----------------


/** \brief Type-safe class wrapper with move semantics.
 */
#define AUTOCOM_SAFE_MOVE(safe)                                         \
    void setVariant(VARIANT &variant,                                   \
        safe && value)

/** \brief Type-safe class wrapper with copy semantics.
 */
#define AUTOCOM_SAFE_COPY(safe)                                         \
    void setVariant(VARIANT &variant,                                   \
        safe const &value)

/** \brief Define a setter for a C primitive type.
 */
#define AUTOCOM_SET_PRIMITIVE(type)                                     \
    void setVariant(VARIANT &variant,                                   \
        type const value)

/** \brief Define a setter for a custom type using move semantics.
 */
#define AUTOCOM_SET_MOVE(type)                                          \
    void setVariant(VARIANT &variant,                                   \
        type &&value)

/** \brief Define a setter for a custom type using copy semantics.
 */
#define AUTOCOM_SET_COPY(type)                                          \
    void setVariant(VARIANT &variant,                                   \
        type const &value)

//template <template <typename...> class C>
//void setRvArg(DISPPARAMS& params,
//    const size_t index,
//    const C<char> &c)
//{
//    params.rgvarg[index].vt = VT_ARRAY | VT_I1;
//    aPropVar[cProp].caub.cElems = pmsgprops->aPropVar[i].caub.cElems;
//    aPropVar[cProp].caub.pElems = pmsgprops->aPropVar[i].caub.pElems;
//     TODO:
//}



///** \brief Copy semantics to define a dispparams setter by value.
// */
//#define DISPPARAMS_MOVE_ARRAY(type, vartype)                            \
//                                                                        \
//    void setRvArg(DISPPARAMS& params,                            \
//        const size_t index,                                             \
//        type const &t)                                                  \
//    {                                                                   \
//        params.rgvarg[index].vt = vartype;                              \
//        params.rgvarg[index].parray = t;                                \
//    }


/** \brief Generalized type-safe wrappers for a given type.
 *
 *  Since the type is specialized for TypeWrapper, which provides
 *  a conversion-safe wrapper, any automation type can be passed.
 *
 *  AUTOCOM_SAFE_SETTER(BOOL, boolVal, boolVal)
 */
#define AUTOCOM_SAFE_SETTER(safe)                                       \
    AUTOCOM_SAFE_MOVE(safe);                                            \
    AUTOCOM_SAFE_COPY(safe);                                            \
    AUTOCOM_SAFE_COPY(safe##Ref)


/** \brief Generalized dispparams setter overloaded for a primitive type.
 *
 *  AUTOCOM_PRIMITIVE_SETTER(CHAR, VT_I1, cVal)
 */
#define AUTOCOM_PRIMITIVE_SETTER(type)                                  \
    AUTOCOM_SET_PRIMITIVE(type);                                        \
    AUTOCOM_SET_PRIMITIVE(type*)


/** \brief Generalized dispparams setter overloaded for a primitive type.
 *
 *  AUTOCOM_CLASS_SETTER(CURRENCY, VT_CY, cyVal)
 */
#define AUTOCOM_CLASS_SETTER(type)                                      \
    AUTOCOM_SET_MOVE(type);                                             \
    AUTOCOM_SET_COPY(type);                                             \
    AUTOCOM_SET_MOVE(type*);                                            \
    AUTOCOM_SET_COPY(type*)

// FUNCTIONS -- SETTERS
// --------------------


/** \brief Set a null parameter.
 */
void setVariant(VARIANT &variant,
    std::nullptr_t value);

/** \brief Set a BSTR value from a std::string.
 */
void setVariant(VARIANT &variant,
    const std::string &value);

/** \brief Set a BSTR value from a std::wstring.
 */
void setVariant(VARIANT &variant,
    const std::wstring &value);

/** \brief Set a pointer to a BSTR.
 */
void setVariant(VARIANT &variant,
    BSTR *value);

/** \brief Set a pointer to an IUnknown instance.
 */
void setVariant(VARIANT &variant,
    IUnknown *value);

/** \brief Set a pointer to an IUnknown pointer.
 */
void setVariant(VARIANT &variant,
    IUnknown **value);

/** \brief Set a pointer to an IDispatch instance.
 */
void setVariant(VARIANT &variant,
    IDispatch *value);

/** \brief Set a pointer to an IDispatch pointer.
 */
void setVariant(VARIANT &variant,
    IDispatch **value);

/** \brief Set a pointer to a VARIANT pointer.
 */
void setVariant(VARIANT &variant,
    VARIANT *value);


// GENERIC
AUTOCOM_PRIMITIVE_SETTER(CHAR);
AUTOCOM_PRIMITIVE_SETTER(UCHAR);
AUTOCOM_PRIMITIVE_SETTER(SHORT);
AUTOCOM_PRIMITIVE_SETTER(USHORT);
AUTOCOM_PRIMITIVE_SETTER(INT);
AUTOCOM_PRIMITIVE_SETTER(UINT);
AUTOCOM_PRIMITIVE_SETTER(LONG);
AUTOCOM_PRIMITIVE_SETTER(ULONG);
AUTOCOM_PRIMITIVE_SETTER(FLOAT);
AUTOCOM_PRIMITIVE_SETTER(DOUBLE);
AUTOCOM_CLASS_SETTER(CURRENCY);

// SAFE
AUTOCOM_SAFE_SETTER(SafeBool);
AUTOCOM_SAFE_SETTER(SafeChar);
AUTOCOM_SAFE_SETTER(SafeUChar);
AUTOCOM_SAFE_SETTER(SafeShort);
AUTOCOM_SAFE_SETTER(SafeUShort);
AUTOCOM_SAFE_SETTER(SafeInt);
AUTOCOM_SAFE_SETTER(SafeUInt);
AUTOCOM_SAFE_SETTER(SafeLong);
AUTOCOM_SAFE_SETTER(SafeULong);
AUTOCOM_SAFE_SETTER(SafeFloat);
AUTOCOM_SAFE_SETTER(SafeDouble);
AUTOCOM_SAFE_SETTER(SafeCurrency);
//AUTOCOM_SAFE_SETTER(SafeBstr);

// TOOD: finish everything here...
// https://msdn.microsoft.com/en-us/library/windows/desktop/aa380072(v=vs.85).aspx

//// TODO: VT_VECTOR
//// TODO: VT_ARRAY

//#ifdef _WIN64
//    // 64 BIT ONLY
//    DISPPARAMS_SETTER(LONGLONG, VT_I8, hVal)
//    DISPPARAMS_SETTER(ULONGLONG, VT_UI8, uhVal)
//    DISPPARAMS_SETTER(FILETIME, VT_FILETIME, filetime)
//#endif          // WIN64

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

/** \brief Define a getter for a type.
 */
#define AUTOCOM_GET(type)                                               \
    void getVariant(VARIANT &variant,                                   \
        type *t)

/** \brief Define a safe getter for a type.
 */
#define AUTOCOM_SAFE_GET(safe)                                          \
    void getVariant(VARIANT &variant,                                   \
        safe &t)

/** \brief Define a getter for type by value and by reference.
 */
#define AUTOCOM_GETTER(type)                                            \
    AUTOCOM_GET(type);                                                  \
    AUTOCOM_GET(type*)

/** \brief Define type-safe getter for type from value and from reference.
 */
#define AUTOCOM_SAFE_GETTER(safe)                                       \
    AUTOCOM_SAFE_GET(safe##Ref);                                        \
    AUTOCOM_SAFE_GET(safe##RefRef)

// FUNCTIONS -- GETTERS
// --------------------

/** \brief Set a pointer to a VARIANT pointer.
 */
void getVariant(VARIANT &variant,
    VARIANT **value);


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
AUTOCOM_GETTER(CURRENCY);
AUTOCOM_GETTER(BSTR);
AUTOCOM_GETTER(IUnknown*);
AUTOCOM_GETTER(IDispatch*);

// SAFE
AUTOCOM_SAFE_GETTER(SafeBool);
AUTOCOM_SAFE_GETTER(SafeChar);
AUTOCOM_SAFE_GETTER(SafeUChar);
AUTOCOM_SAFE_GETTER(SafeShort);
AUTOCOM_SAFE_GETTER(SafeUShort);
AUTOCOM_SAFE_GETTER(SafeInt);
AUTOCOM_SAFE_GETTER(SafeUInt);
AUTOCOM_SAFE_GETTER(SafeLong);
AUTOCOM_SAFE_GETTER(SafeULong);
AUTOCOM_SAFE_GETTER(SafeFloat);
AUTOCOM_SAFE_GETTER(SafeDouble);
AUTOCOM_SAFE_GETTER(SafeCurrency);
//AUTOCOM_SAFE_GETTER(SafeBstr);


// CLEANUP -- GETTERS
// ------------------

#undef AUTOCOM_GET
#undef AUTOCOM_SAFE_GET
#undef AUTOCOM_GETTER
#undef AUTOCOM_SAFE_GETTER

// OBJECTS
// -------


/** \brief C++ wrapper for VARIANT.
 */
class Variant: public VARIANT
{
public:
    Variant();
    ~Variant();

    void init();
    void clear();
};


// TYPES
// -----

typedef std::vector<Variant> VariantList;

}   /* autocom */
