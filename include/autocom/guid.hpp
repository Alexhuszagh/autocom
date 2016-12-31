//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCOM
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
    GUID id;

    friend class Dispatch;

    void open(const std::string &string);

public:
    Guid() = default;
    Guid(const Guid &other) = delete;
    Guid & operator=(const Guid&) = delete;
    Guid(Guid&&) = default;
    Guid & operator=(Guid&&) = default;

    Guid(const GUID &guid);
    Guid(const char *cstring);
    Guid(const char *array,
        const size_t length);
    Guid(const std::string &string);

    // DATA
    static Guid fromProgid(const std::string &string);
    std::string toProgid();
    static Guid fromClsid(const std::string &string);
    std::string toClsid();
    static Guid fromIid(const std::string &string);
    std::string toIid();
    std::string string() const;

    friend bool operator==(const Guid &left,
        const Guid &right);
    friend bool operator!=(const Guid &left,
        const Guid &right);
};


}   /* autocom */
