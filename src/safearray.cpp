//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief COM SafeArray wrapper.
 */

#include "autocom.hpp"


namespace autocom
{
// OBJECTS
// -------


/** \brief Initialize from copies bounds.
 */
SafeArrayBound::SafeArrayBound(const SAFEARRAYBOUND &bound):
    SAFEARRAYBOUND(bound)
{}


/** \brief Initialize from moved bounds.
 */
SafeArrayBound::SafeArrayBound(SAFEARRAYBOUND &&bound):
    SAFEARRAYBOUND(bound)
{}


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

//
//
///** \brief Inherit existing SAFEARRAY pointer.
// */
//SafeArray::SafeArray(SAFEARRAY *array):
//    array(array, SafeArrayDestroy)
//{}
//
//
///** \brief Copy existing SAFEARRAY into new object.
// */
//SAFEARRAY * SafeArray::copy() const
//{
//    SAFEARRAY *out;
//    if (FAILED(SafeArrayCopy(array.get(), &out))) {
//        throw ComFunctionError("SafeArrayCopy()");
//    }
//
//    return out;
//}
//
//
///** \brief Close existing SAFEARRAY wrapper.
// */
//void SafeArray::close()
//{
//    array.reset();
//}
//
//
///** \brief Create new array.
// */
//void SafeArray::create(VARTYPE vt,
//    UINT dimensions,
//    SafeArrayBound *bound)
//{
//    close();
//    auto *ptr = SafeArrayCreate(vt, dimensions, &bound->bound);
//    array.reset(ptr, SafeArrayDestroy);
//}
//
//
///** \brief Access data and lock array (can be called sequentially).
// */
//void * SafeArray::access()
//{
//    void *data = nullptr;
//    if (FAILED(SafeArrayAccessData(array.get(), &data))) {
//        throw ComFunctionError("SafeArrayAccessData()");
//    }
//
//    return data;
//}
//
//
///** \brief Unaccess data and unlock array.
// */
//void SafeArray::unaccess()
//{
//    if (FAILED(SafeArrayUnaccessData(array.get()))) {
//        throw ComFunctionError("SafeArrayUnaccessData()");
//    }
//}
//
//
///** \brief Get element from array.
// */
//void * SafeArray::get(LONG *indices)
//{
//    void *data = nullptr;
//    if (FAILED(SafeArrayGetElement(array.get(), indices, data))) {
//        throw ComFunctionError("SafeArrayGetElement()");
//    }
//
//    return data;
//}
//
//
///** \brief Put element in array.
// */
//void SafeArray::put(LONG *indices,
//    void *data)
//{
//    if (FAILED(SafeArrayPutElement(array.get(), indices, data))) {
//        throw ComFunctionError("SafeArrayPutElement()");
//    }
//}
//
//
///** \brief Resize outter most dimension of array.
// */
//void SafeArray::resize(SafeArrayBound *bound)
//{
//    if (FAILED(SafeArrayRedim(array.get(), &bound->bound))) {
//        throw ComFunctionError("SafeArrayUnaccessData()");
//    }
//}


}   /* autocom */
