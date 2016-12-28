//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCom
 *  \brief Iterator for IEnumVARIANT interfaces.
 */

#include "autocom.hpp"


namespace autocom
{
// OBJECTS
// -------


/** \brief Initializer list constructor.
 */
Iterator::Iterator(std::weak_ptr<IEnumVARIANT> ppv):
    ppv(ppv)
{}


/** \brief Dereference iterator.
 */
DispatchBase & Iterator::operator*()
{
    return dispatch;
}


/** \brief Dereference iterator.
 */
const DispatchBase & Iterator::operator*() const
{
    return dispatch;
}


/** \brief Dereference iterator.
 */
DispatchBase * Iterator::operator->()
{
    return &dispatch;
}


/** \brief Dereference iterator.
 */
const DispatchBase * Iterator::operator->() const
{
    return &dispatch;
}


/** \brief Pre-increment operator.
 */
Iterator & Iterator::operator++()
{
    VARIANT result;
    ULONG fetched;
    auto ev = ppv.lock();
    if (ev && SUCCEEDED(ev->Next(1, &result, &fetched))) {
        dispatch.open(result.pdispVal);
    } else {
        dispatch.open(nullptr);
    }

    return *this;
}


/** \brief Post-increment operator.
 */
Iterator Iterator::operator++(int)
{
    Iterator copy(*this);
    operator++();

    return copy;
}


/** \brief Equality operator.
 */
bool Iterator::operator==(const Iterator& other) const
{
    return (ppv.lock() == other.ppv.lock()) && (dispatch == other.dispatch);
}


/** \brief Inequality operator.
 */
bool Iterator::operator!=(const Iterator& other) const
{
    return !operator==(other);
}


}   /* autocom */

