//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCom
 *  \brief C++ handle around IEnumVARIANT.
 */

#pragma once

#include "iterator.hpp"

#include <oaidl.h>

#include <memory>


namespace autocom
{
// OBJECTS
// -------


/** \brief COM object wrapper for the IEnumVARIANT model.
 */
class EnumVariant
{
protected:
    std::shared_ptr<IEnumVARIANT> ppv;

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

    EnumVariant(IDispatch *dispatch);

    void open(IDispatch *dispatch);

    iterator begin();
    iterator end();
};


}   /* autocom */
