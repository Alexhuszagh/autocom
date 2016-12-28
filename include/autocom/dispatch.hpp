//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCom
 *  \brief C++ handle around IDispatch.
 */

#pragma once

#include "dispparams.hpp"
#include "enum.hpp"
#include "guid.hpp"
#include "util.hpp"


namespace autocom
{
// OBJECTS
// -------


/** \brief COM object wrapper for the IDispatch model.
 */
class Dispatch: public DispatchBase
{
protected:
    friend class EnumVariant;

    using DispatchBase::invoke;

public:
    Dispatch();
    Dispatch(const Dispatch &other);
    Dispatch & operator=(const Dispatch &other);
    Dispatch(Dispatch &&other);
    Dispatch & operator=(Dispatch &&other);
    ~Dispatch();

    Dispatch(const Guid &guid,
        LPUNKNOWN outter = nullptr,
        DWORD context = CLSCTX_INPROC_SERVER);

    void open(const Guid &guid,
        LPUNKNOWN outter = nullptr,
        DWORD context = CLSCTX_INPROC_SERVER);

    using DispatchBase::get;
    using DispatchBase::put;
    using DispatchBase::putref;
    using DispatchBase::method;

    template <typename... Ts>
    EnumVariant iter(Ts&&... ts);
};


// IMPLEMENTATION
// --------------


/** \brief Iterate over elements from method call.
 */
template <typename... Ts>
EnumVariant Dispatch::iter(Ts&&... ts)
{
    IDispatch *dispatch = DispatchBase::ppv.get();
    Variant result;
    if (sizeof...(Ts)) {
        // fetch dispatcher
        if (!invoke(GET, &result, AUTOCOM_FWD(ts)...)) {
            return EnumVariant();
        }
        dispatch = result.pdispVal;
    }

    return EnumVariant(dispatch);
}


}   /* autocom */
