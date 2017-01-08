//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief Base definitions for COM objects.
 */

#pragma once

#include "dispparams.hpp"
#include "util/define.hpp"
#include "util/exception.hpp"
#include "util/shared_ptr.hpp"

#include <initguid.h>
#include <dispex.h>


namespace autocom
{
// TYPES
// -----

typedef MEMBERID Function;
typedef std::string Name;
typedef std::wstring WName;
typedef std::pair<Variant, bool> MethodResult;

// FUNCTIONS
// ---------


/** \brief Initialize COM context for current thread.
 */
void initialize();

/** \brief Uninitialize COM context for current thread.
 */
void uninitialize();


/** \brief Check if two COM handles point to same object.
 *
 *  Two identical COM objects can have different memory addresses,
 *  so the IID_IUnknown interface must be queried. In the case of
 *  Internet Explorer COM objects, two identical COM objects
 *  instances can have different IID_IUnknown addresses, and must
 *  be checked using `IObjectIdentity::IsEqualObject`.
 */
template <typename ObjectPtr>
bool equalObject(ObjectPtr left,
    ObjectPtr right)
{
    if (left == right) {
        return true;
    }

    bool equal = false;
    IObjectIdentity *identity = nullptr;
    IUnknown *l = nullptr;
    IUnknown *r = nullptr;
    if (left && right) {
        left->QueryInterface(IID_IUnknown, (void**) &l);
        right->QueryInterface(IID_IUnknown, (void**) &r);
        if (l == r) {
            equal = true;
        } else if (l && r) {
            if (SUCCEEDED(l->QueryInterface(IID_IObjectIdentity, (void **) &identity))) {
                equal = identity->IsEqualObject(r) == S_OK;
            }
        }
    }

    // cleanup
    if (identity) {
        identity->Release();
    }
    if (l) {
        l->Release();
    }
    if (r) {
        r->Release();
    }

    return equal;
}


// OBJECTS
// -------


/** \brief COM object wrapper for the IDispatch (late-binding) model.
 */
class DispatchBase
{
protected:
    SharedPointer<IDispatch> ppv;

    Function getFunction(const Bstr &name);

    template <typename... Ts>
    bool invoke(DispatchFlags flags,
        VARIANT *result,
        const Function id,
        Ts&&... ts);

    template <typename... Ts>
    bool invoke(DispatchFlags flags,
        VARIANT *result,
        const Bstr &name,
        Ts&&... ts);

    template <typename... Ts>
    MethodResult get_(Ts&&... ts);

    template <typename... Ts>
    MethodResult put_(Ts&&... ts);

    template <typename... Ts>
    MethodResult putref_(Ts&&... ts);

    template <typename... Ts>
    MethodResult method_(Ts&&... ts);

    friend bool operator==(const DispatchBase &left,
        const DispatchBase &right);
    friend bool operator!=(const DispatchBase &left,
        const DispatchBase &right);

public:
    DispatchBase() = default;
    DispatchBase(const DispatchBase&) = default;
    DispatchBase & operator=(const DispatchBase&) = default;
    DispatchBase(DispatchBase&&) = default;
    DispatchBase & operator=(DispatchBase&&) = default;

    DispatchBase(IDispatch *dispatch);

    void open(IDispatch *dispatch);
    void reset();

    // INTERNAL VARIANT
    template <typename... Ts>
    bool get(Ts&&... ts);

    template <typename... Ts>
    bool put(Ts&&... ts);

    template <typename... Ts>
    bool putref(Ts&&... ts);

    template <typename... Ts>
    bool method(Ts&&... ts);

    // CUSTOM VARIANT
    template <typename... Ts>
    Variant getV(Ts&&... ts);

    template <typename... Ts>
    Variant putV(Ts&&... ts);

    template <typename... Ts>
    Variant putrefV(Ts&&... ts);

    template <typename... Ts>
    Variant methodV(Ts&&... ts);

    explicit operator bool() const;
    IDispatch & operator*();
    const IDispatch & operator*() const;
    IDispatch * operator->();
    const IDispatch * operator->() const;
};


/** \brief Wrapper for the CoClass (early-binding) model.
 *
 *  Automates class construction and destruction, forcing per-thread
 *  initialization, creating the proper object from the CLSID and IID,
 *  and deinitializing after object cleanup.
 */
template <
    typename T,
    typename Interface = T
>
class ComObject: public SharedPointer<Interface>
{
protected:
    typedef SharedPointer<Interface> Base;
    typedef ComObject<Interface> This;

public:
    ComObject();
    ~ComObject();

    void open();
    void close();
};


// IMPLEMENTATION
// --------------


/** \brief Call dispatch method by function ID.
 */
template <typename... Ts>
bool DispatchBase::invoke(DispatchFlags flags,
    VARIANT *result,
    const Function id,
    Ts&&... ts)
{
    DispParams dp;
    dp.setArgs(AUTOCOM_FWD(ts)...);
    dp.setFlags(flags);

    return SUCCEEDED(ppv->Invoke(id, IID_NULL, LOCALE_USER_DEFAULT, FROM_ENUM(flags), dp.params(), result, nullptr, nullptr));
}


/** \brief Call dispatch method by function name.
 */
template <typename... Ts>
bool DispatchBase::invoke(DispatchFlags flags,
    VARIANT *result,
    const Bstr &name,
    Ts&&... ts)
{
    return invoke(flags, result, getFunction(name), AUTOCOM_FWD(ts)...);
}


template <typename... Ts>
MethodResult DispatchBase::get_(Ts&&... ts)
{
    static_assert(sizeof...(Ts) == 2, "Must provide function and reference");

    Variant result;
    if (invoke(GET, &result, packGet<0>(AUTOCOM_FWD(ts)...))) {
        autocom::get(result, packGet<1>(AUTOCOM_FWD(ts)...));
        return std::make_pair(result, true);
    }

    return std::make_pair(result, false);
}


template <typename... Ts>
MethodResult DispatchBase::put_(Ts&&... ts)
{
    static_assert(sizeof...(Ts) >= 1, "Must provide function identifier");

    Variant result;
    auto status = invoke(PUT, &result, AUTOCOM_FWD(ts)...);

    return std::make_pair(result, status);
}


template <typename... Ts>
MethodResult DispatchBase::putref_(Ts&&... ts)
{
    static_assert(sizeof...(Ts) >= 1, "Must provide function identifier");

    Variant result;
    auto status = invoke(PUTREF, &result, AUTOCOM_FWD(ts)...);

    return std::make_pair(result, status);
}


template <typename... Ts>
MethodResult DispatchBase::method_(Ts&&... ts)
{
    static_assert(sizeof...(Ts) >= 1, "Must provide function identifier");

    Variant result;
    auto status = invoke(METHOD, &result, AUTOCOM_FWD(ts)...);

    return std::make_pair(result, status);
}


/** \brief Call get with return status.
 */
template <typename... Ts>
bool DispatchBase::get(Ts&&... ts)
{
    return get_(AUTOCOM_FWD(ts)...).second;
}


/** \brief Call put with return status.
 */
template <typename... Ts>
bool DispatchBase::put(Ts&&... ts)
{
    return put_(AUTOCOM_FWD(ts)...).second;
}


/** \brief Call putref with return status.
 */
template <typename... Ts>
bool DispatchBase::putref(Ts&&... ts)
{
    return putref_(AUTOCOM_FWD(ts)...).second;
}


/** \brief Call method with return status.
 */
template <typename... Ts>
bool DispatchBase::method(Ts&&... ts)
{
    return method_(AUTOCOM_FWD(ts)...).second;
}


/** \brief Call get with return variant.
 */
template <typename... Ts>
Variant DispatchBase::getV(Ts&&... ts)
{
    auto value = get_(AUTOCOM_FWD(ts)...);
    if (!value.second) {
        throw ComMethodError("IDispatch", "Invoke(DISPATCH_PROPERTYGET, ...)");
    }

    return value.first;
}


/** \brief Call put with return variant.
 */
template <typename... Ts>
Variant DispatchBase::putV(Ts&&... ts)
{
    auto value = put_(AUTOCOM_FWD(ts)...);
    if (!value.second) {
        throw ComMethodError("IDispatch", "Invoke(DISPATCH_PROPERTYPUT, ...)");
    }

    return value.first;
}


/** \brief Call putref with return variant.
 */
template <typename... Ts>
Variant DispatchBase::putrefV(Ts&&... ts)
{
    auto value = putref_(AUTOCOM_FWD(ts)...);
    if (!value.second) {
        throw ComMethodError("IDispatch", "Invoke(DISPATCH_PROPERTYPUTREF, ...)");
    }

    return value.first;
}


/** \brief Call method with return variant.
 */
template <typename... Ts>
Variant DispatchBase::methodV(Ts&&... ts)
{
    auto value = method_(AUTOCOM_FWD(ts)...);
    if (!value.second) {
        throw ComMethodError("IDispatch", "Invoke(DISPATCH_METHOD, ...)");
    }

    return value.first;
}


/** \brief Initialize interface on construction.
 */
template <
    typename T,
    typename Interface
>
ComObject<T, Interface>::ComObject()
{
    open();
}


/** \brief Uninitialize COM interface on object destruction.
 */
template <
    typename T,
    typename Interface
>
ComObject<T, Interface>::~ComObject()
{
    close();
}


/** \brief Initialize COM interface.
 */
template <
    typename T,
    typename Interface
>
void ComObject<T, Interface>::open()
{
    initialize();
    Interface *ppv;
    if (FAILED(CoCreateInstance(T::clsid, nullptr, CLSCTX_INPROC_SERVER, Interface::iid, (void **) &ppv))) {
        throw ComFunctionError("CoCreateInstance()");
    }
    Base::reset(ppv);
}


/** \brief Uninitialize COM interface.
 */
template <
    typename T,
    typename Interface
>
void ComObject<T, Interface>::close()
{
    Base::reset();
    uninitialize();
}


}   /* autocom */
