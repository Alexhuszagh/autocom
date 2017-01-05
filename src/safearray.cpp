//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief COM SafeArray wrapper.
 */

#include "autocom/safearray.hpp"


namespace autocom
{
// FUNCTIONS
// ---------


/** \brief IUnknown type-safe wrapper for SafeArrayGetVartype.
 */
VARTYPE getSafeArrayType(const SAFEARRAY *value)
{
    VARTYPE vt;
    auto *array = const_cast<SAFEARRAY*>(value);

    if (array->fFeatures & FADF_UNKNOWN) {
        vt = VT_UNKNOWN;
    } else {
        if (FAILED(SafeArrayGetVartype(array, &vt))) {
            throw ComFunctionError("SafeArrayGetVartype");
        }
    }

    return vt;
}


// OBJECTS
// -------


/** \brief Initialize from copied bounds.
 */
SafeArrayBound::SafeArrayBound(const SAFEARRAYBOUND &bound):
    SAFEARRAYBOUND(bound)
{}


/** \brief Initialize from moved bounds.
 */
SafeArrayBound::SafeArrayBound(SAFEARRAYBOUND &&bound):
    SAFEARRAYBOUND(std::move(bound))
{}


/** \brief Initialize C-style array bounds by size.
 */
SafeArrayBound::SafeArrayBound(const size_t size)
{
    lLbound = 0;
    cElements = size;
}


/** \brief Initialize bounds with lower and upper range.
 */
SafeArrayBound::SafeArrayBound(const size_t lower,
    const size_t upper)
{
    lLbound = lower;
    cElements = upper;
}


/** \brief Get number of elements in bound.
 */
ULONG SafeArrayBound::size() const
{
    return cElements - lLbound;
}


/** \brief Get lower bound of elements.
 */
LONG & SafeArrayBound::lower()
{
    return lLbound;
}


/** \brief Get upper bound of elements.
 */
ULONG & SafeArrayBound::upper()
{
    return cElements;
}

}   /* autocom */
