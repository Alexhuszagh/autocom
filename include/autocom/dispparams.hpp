//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCom
 *  \brief Templates to automate dispparams construction.
 */

#pragma once

#include "except.hpp"
#include "util.hpp"

#include <oaidl.h>
#include <windows.h>
#include <wtypes.h>

#include <cstdint>
#include <cstddef>
#include <vector>


namespace autocom
{
// ENUM
// ----


/** \brief Strongly-typed enum wrapper for dispatch identifiers.
 */
enum class Dispatch: WORD
{
    METHOD          = DISPATCH_METHOD,
    PROPERTYGET     = DISPATCH_PROPERTYGET,
    PROPERTYPUT     = DISPATCH_PROPERTYPUT,
    PROPERTYPUTREF  = DISPATCH_PROPERTYPUTREF,
};

AUTOCOM_FLAGS(Dispatch)

// MACROS
// ------


/** \brief Generalized dispparams setter overloaded for a custom type.
 *
 *  The setter can be specialized for any type, vartype, and field,
 *  as long as no ambiguous overloads are specialized.
 *
 *  DISPPARAMS_SETTER(int8_t, VT_I1, cVal)
 */
#define DISPPARAMS_SETTER(type, vartype, field)                         \
                                                                        \
    inline void setRvArg(DISPPARAMS& params,                            \
        const size_t index,                                             \
        type &&t)                                                       \
    {                                                                   \
        params.rgvarg[index].vt = vartype;                              \
        params.rgvarg[index].field = std::move(t);                      \
    }                                                                   \
                                                                        \
    inline void setRvArg(DISPPARAMS& params,                            \
        const size_t index,                                             \
        const type &t)                                                  \
    {                                                                   \
        params.rgvarg[index].vt = vartype;                              \
        params.rgvarg[index].field = t;                                 \
    }                                                                   \
                                                                        \
    inline void setRvArg(DISPPARAMS& params,                            \
        const size_t index,                                             \
        type * const &t)                                                \
    {                                                                   \
        params.rgvarg[index].vt = vartype | VT_BYREF;                   \
        params.rgvarg[index].p##field = t;                              \
    }                                                                   \
                                                                        \
    inline void setRvArg(DISPPARAMS& params,                            \
        const size_t index,                                             \
        type *&&t)                                                      \
    {                                                                   \
        params.rgvarg[index].vt = vartype | VT_BYREF;                   \
        params.rgvarg[index].p##field = std::move(t);                   \
    }


// FUNCTIONS
// ---------


DISPPARAMS_SETTER(CHAR, VT_I1, cVal)
DISPPARAMS_SETTER(UCHAR, VT_UI1, bVal)
DISPPARAMS_SETTER(SHORT, VT_I2, iVal)
DISPPARAMS_SETTER(USHORT, VT_UI2, uiVal)
DISPPARAMS_SETTER(LONG, VT_I4, lVal)
DISPPARAMS_SETTER(ULONG, VT_UI4, ulVal)
DISPPARAMS_SETTER(FLOAT, VT_R4, fltVal)
DISPPARAMS_SETTER(DOUBLE, VT_R8, dblVal)
DISPPARAMS_SETTER(CURRENCY, VT_CY, cyVal)
DISPPARAMS_SETTER(BSTR, VT_BSTR, bstrVal)

#ifdef _WIN64
    // 64 BIT ONLY
    DISPPARAMS_SETTER(LONGLONG, VT_I8, hVal)
    DISPPARAMS_SETTER(ULONGLONG, VT_UI8, uhVal)
    DISPPARAMS_SETTER(FILETIME, VT_FILETIME, filetime)
#endif          // WIN64


/** \brief Forward parameter to rvargs.
 */
template <typename T>
void setArg(DISPPARAMS& params,
    const size_t index,
    T &&t)
{
    setRvArg(params, index, AUTOCOM_FWD(t));
}


/** \brief Forward parameters in argument order.
 *
 *  DISPPARAMS::rgvargs takes arguments in reverse sorted order,
 *  so the argument list is the index count after unpacking the item.
 *
 *  Example, Arg0->3, Arg1->2, Arg2->1, Arg3->0
 */
template <
    typename T,
    typename... Ts
>
void setArg(DISPPARAMS& params,
    const size_t index,
    T &&t,
    Ts&&... ts)
{
    setArg(params, sizeof...(Ts), AUTOCOM_FWD(t));
    setArg(params, sizeof...(Ts), AUTOCOM_FWD(ts)...);
}


// OBJECTS
// -------


/** \brief Wrapper around DISPPARAMS object.
 */
class DispParams
{
protected:
    DISPPARAMS params;
    std::vector<VARIANT> args;
    DISPID named = DISPID_PROPERTYPUT;

public:
    DispParams();
    DispParams(const DispParams &other);

    ~DispParams();

    // DATA
    void setDispatch(const Dispatch dispatch);

    template <typename... Ts>
    void setArgs(Ts&&... ts);
};


// IMPLEMENTATION
// --------------


/** \brief Set argument list for dispparams.
 */
template <typename... Ts>
void DispParams::setArgs(Ts&&... ts)
{
    constexpr size_t size = sizeof...(Ts);

    args.resize(size);
    params.cArgs = size;
    params.rgvarg = const_cast<VARIANT*>(args.data());
    setArg(params, size, AUTOCOM_FWD(ts)...);
}

// CLEANUP
// -------

#undef DISPPARAMS_SETTER


}   /* autocom */
