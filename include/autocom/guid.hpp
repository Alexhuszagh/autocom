//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCom
 *  \brief COM GUID utilities.
 */

#pragma once

#include <rpc.h>

#include <string>


namespace autocom
{
// OBJECTS
// -------


/** \brief Wrapper around GUID object.
 */
class Guid
{
protected:
    GUID guid;

public:
    Guid();
    Guid(const GUID &guid);
    Guid(const std::string &string);
    Guid(const Guid &other);
    ~Guid();

    // DATA
    static Guid fromProgid(const std::string &string);
    std::string toProgid();
    static Guid fromClsid(const std::string &string);
    std::string toClsid();
    static Guid fromIid(const std::string &string);
    std::string toIid();

    friend bool operator==(const Guid &left,
        const Guid &right);
    friend bool operator!=(const Guid &left,
        const Guid &right);
};


}   /* autocom */
