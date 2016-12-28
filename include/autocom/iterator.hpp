//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCom
 *  \brief Iterator for IEnumVARIANT interfaces.
 */

#pragma once

#include "com.hpp"

#include <iterator>
#include <memory>


namespace autocom
{
// OBJECTS
// -------


/** \brief EnumVARIANT iterator.
 */
class Iterator: public std::iterator<
        std::forward_iterator_tag,
        DispatchBase
    >
{
protected:
    std::weak_ptr<IEnumVARIANT> ppv;
    DispatchBase dispatch;

public:
    Iterator() = default;
    Iterator(Iterator&) = default;
    Iterator & operator=(Iterator&) = default;
    Iterator(Iterator&&) = default;
    Iterator & operator=(Iterator&&) = default;

    Iterator(std::weak_ptr<IEnumVARIANT> ppv);

    DispatchBase & operator*();
    const DispatchBase & operator*() const;
    DispatchBase * operator->();
    const DispatchBase * operator->() const;

    Iterator & operator++();
    Iterator operator++(int);
    bool operator==(const Iterator& other) const;
    bool operator!=(const Iterator& other) const;
};


}   /* autocom */