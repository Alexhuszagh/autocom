//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief C++ handle around IEnumVARIANT.
 */

#pragma once

#include <autocom/iterator.h>
#include <autocom/util/shared_ptr.h>

#include <oaidl.h>


namespace autocom
{
// FUNCTIONS
// ---------

/** \brief Create new handle to IEnumVARIANT object from IDispatch.
 */
IEnumVARIANT * newEnumVariant(IDispatch *dispatch);

// OBJECTS
// -------


/** \brief COM object wrapper for the IEnumVARIANT model.
 */
class EnumVariant
{
protected:
    SharedPointer<IEnumVARIANT> ppv;

    friend bool operator==(const EnumVariant &left,
        const EnumVariant &right);
    friend bool operator!=(const EnumVariant &left,
        const EnumVariant &right);

public:
    typedef Iterator iterator;

    EnumVariant() = default;
    EnumVariant(const EnumVariant&) = default;
    EnumVariant & operator=(const EnumVariant&) = default;
    EnumVariant(EnumVariant&&) = default;
    EnumVariant & operator=(EnumVariant&&) = default;

    EnumVariant(IEnumVARIANT *enumvariant);
    void open(IEnumVARIANT *enumvariant);

    iterator begin();
    iterator end();
};


}   /* autocom */
