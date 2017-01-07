//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief C++ handle around IEnumVARIANT.
 */

#include "autocom/enum.hpp"
#include "autocom/util/exception.hpp"


namespace autocom
{
// FUNCTIONS
// ---------


/** \brief Create new handle to IEnumVARIANT object from IDispatch.
 */
IEnumVARIANT * newEnumVariant(IDispatch *dispatch)
{
    DISPPARAMS dp = {nullptr, nullptr, 0, 0};
    Variant result;

    HRESULT hr = dispatch->Invoke(DISPID_NEWENUM, IID_NULL, LOCALE_USER_DEFAULT, FROM_ENUM(GET), &dp, &result, nullptr, nullptr);
    if (FAILED(hr)) {
        throw ComMethodError("IDispatch", "Invoke(DISPID_NEWENUM, ...)");
    }

    IEnumVARIANT *ppv = nullptr;
    if (result.vt == VT_DISPATCH) {
        hr = result.pdispVal->QueryInterface(IID_IEnumVARIANT, (void**) &ppv);
    } else if (result.vt == VT_UNKNOWN) {
        hr  = result.punkVal->QueryInterface(IID_IEnumVARIANT, (void**) &ppv);
    } else {
        hr = E_NOINTERFACE;
    }

    if (FAILED(hr)) {
        throw ComMethodError("IDispatch", "QueryInterface(IID_IEnumVARIANT, ...)");
    }

    return ppv;
}


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
EnumVariant::EnumVariant(IEnumVARIANT *enumvariant)
{
    open(enumvariant);
}


/** \brief Open handle to IEnumVARIANT model.
 *
 *  Variant's destructor, which calls VariantClear, automatically cleans
 *  up the dispatcher.
 */
void EnumVariant::open(IEnumVARIANT *enumvariant)
{
    ppv.reset(enumvariant);
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
