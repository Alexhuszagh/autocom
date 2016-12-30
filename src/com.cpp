//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCom
 *  \brief Base definitions for COM objects.
 */

#include "autocom.hpp"

#include <mutex>
#include <thread>


namespace autocom
{
// CONSTANTS
// ---------

/** Initialized instance counts in the current thread.
 */
thread_local long COUNT = 0;

// FUNCTIONS
// ---------


/** \brief Initialize COM context for current thread.
 */
void initialize()
{
    if (COUNT <= 0) {
        CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        COUNT = 1;
    } else {
        ++COUNT;
    }
}


/** \brief Uninitialize COM context for current thread.
 */
void uninitialize()
{
    if (COUNT <= 1) {
        CoUninitialize();
        COUNT = 0;
    } else {
        --COUNT;
    }
}


// OBJECTS
// -------


/** \brief Get dispatch identifier from function identifier.
 */
Function DispatchBase::getFunction(const Name &name)
{
    DISPID id;
    WORD flags = DISPATCH_METHOD;
    LCID locale = LOCALE_USER_DEFAULT;
    auto wide = WIDE(name);
    LPOLESTR string = const_cast<wchar_t*>(wide.data());
    if (FAILED(ppv->GetIDsOfNames(IID_NULL, &string, flags, locale, &id))) {
        throw ComMethodError("IDispatch", "GetIDsOfNames(IID_NULL, ...)");
    }

    return id;
}


/** \brief Equality operator.
 */
bool operator==(const DispatchBase &left,
    const DispatchBase &right)
{
    return equalObject(left.ppv, right.ppv);
}


/** \brief Inequality operator.
 */
bool operator!=(const DispatchBase &left,
    const DispatchBase &right)
{
    return !operator==(left, right);
}


/** \brief Inherit dispatcher.
 */
DispatchBase::DispatchBase(IDispatch *dispatch)
{
    open(dispatch);
}


/** \brief Open handle to IDispatch COM object.
 */
void DispatchBase::open(IDispatch *dispatch)
{
    if (dispatch) {
        ppv.reset(dispatch, destroy<IDispatch>);
    } else {
        ppv.reset();
    }
}


/** \brief Check if class instantiates valid dispatcher.
 */
DispatchBase::operator bool() const
{
    return bool(ppv);
}


}   /* autocom */
