//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCom
 *  \brief C++ handle around IEnumVARIANT.
 */

#include "autocom.hpp"


namespace autocom
{
// OBJECTS
// -------


/** \brief Equality operator.
 */
bool operator==(const EnumVariant &left,
    const EnumVariant &right)
{
    return equalObject(left.ppv, right.ppv);
}


/** \brief Inequality operator.
 */
bool operator!=(const EnumVariant &left,
    const EnumVariant &right)
{
    return !operator==(left, right);
}


/** \brief Initialize class from IDispatch.
 */
EnumVariant::EnumVariant(IDispatch *dispatch)
{
    open(dispatch);
}


/** \brief Open handle to IEnumVARIANT model.
 *
 *  Variant's destructor, which calls VariantClear, automatically cleans
 *  up the dispatcher.
 */
void EnumVariant::open(IDispatch *dispatch)
{
    DISPPARAMS dp = {nullptr, nullptr, 0, 0};
    Variant result;

    HRESULT hr = dispatch->Invoke(DISPID_NEWENUM, IID_NULL, LOCALE_USER_DEFAULT, FROM_ENUM(GET), &dp, &result, nullptr, nullptr);
    if (FAILED(hr)) {
        throw IEnumVariantInvokeError();
    }

    IEnumVARIANT *ev;
    if (result.vt == VT_DISPATCH) {
        hr = result.pdispVal->QueryInterface(IID_IEnumVARIANT, (void**) &ev);
    } else if (result.vt == VT_UNKNOWN) {
        hr  = result.punkVal->QueryInterface(IID_IEnumVARIANT, (void**) &ev);
    } else {
        hr = E_NOINTERFACE;
    }

    if (FAILED(hr)) {
        throw IEnumVariantQueryError();
    }
    ppv.reset(ev, destroy<IEnumVARIANT>);
}


/** \brief Get iterator at start of iterator.
 */
auto EnumVariant::begin()
    -> iterator
{
    iterator it(ppv);
    ++it;

    return it;
}


/** \brief Get iterator past end of iterator.
 */
auto EnumVariant::end()
    -> iterator
{
    return iterator(ppv);
}


}   /* autocom */
