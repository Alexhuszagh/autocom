//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief Base definitions for COM objects.
 */

#pragma once

#include "dispparams.hpp"
#include "guid.hpp"

#include <initguid.h>
#include <dispex.h>

#include <memory>


namespace autocom
{
// TYPES
// -----

typedef MEMBERID Function;
typedef std::string Name;

// FUNCTIONS
// ---------


/** \brief Initialize COM context for current thread.
 */
void initialize();

/** \brief Uninitialize COM context for current thread.
 */
void uninitialize();


/** \brief Destroy COM object.
 */
template <typename T>
void destroy(T *t)
{
    t->Release();
}


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


/** \brief COM object wrapper for the IDispatch model.
 */
class DispatchBase
{
protected:
    std::shared_ptr<IDispatch> ppv;

    Function getFunction(const Name &name);

    template <typename... Ts>
    bool invoke(DispatchFlags flags,
        VARIANT *result,
        const Function id,
        Ts&&... ts);

    template <typename... Ts>
    bool invoke(DispatchFlags flags,
        VARIANT *result,
        const Name &name,
        Ts&&... ts);

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

    template <typename... Ts>
    bool get(Ts&&... ts);

    template <typename... Ts>
    bool put(Ts&&... ts);

    template <typename... Ts>
    bool putref(Ts&&... ts);

    template <typename... Ts>
    bool method(Ts&&... ts);

    explicit operator bool() const;
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
    const Name &name,
    Ts&&... ts)
{
    return invoke(flags, result, getFunction(name), AUTOCOM_FWD(ts)...);
}


/** \brief Call get with function ID.
 */
template <typename... Ts>
bool DispatchBase::get(Ts&&... ts)
{
    static_assert(sizeof...(Ts) == 2, "Must provide function and reference");

    VARIANT result;
    if (invoke(GET, &result, packGet<0>(AUTOCOM_FWD(ts)...))) {
        autocom::get(result, packGet<1>(AUTOCOM_FWD(ts)...));
        return true;
    }

    return false;
}


/** \brief Call put.
 */
template <typename... Ts>
bool DispatchBase::put(Ts&&... ts)
{
    static_assert(sizeof...(Ts) >= 1, "Must provide function identifier");

    Variant result;
    return invoke(PUT, &result, AUTOCOM_FWD(ts)...);
}


/** \brief Call put by reference.
 */
template <typename... Ts>
bool DispatchBase::putref(Ts&&... ts)
{
    static_assert(sizeof...(Ts) >= 1, "Must provide function identifier");

    Variant result;
    return invoke(PUTREF, &result, AUTOCOM_FWD(ts)...);
}


/** \brief Call method.
 */
template <typename... Ts>
bool DispatchBase::method(Ts&&... ts)
{
    static_assert(sizeof...(Ts) >= 1, "Must provide function identifier");

    Variant result;
    return invoke(METHOD, &result, AUTOCOM_FWD(ts)...);
}

}   /* autocom */
