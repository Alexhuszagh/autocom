//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief COM GUID utilities.
 */

#pragma once

#include <autocom/bstr.h>


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

    void open(const Bstr &string);

public:
    Guid() = default;
    Guid(const Guid &other) = default;
    Guid & operator=(const Guid&) = default;
    Guid(Guid&&) = default;
    Guid & operator=(Guid&&) = default;

    Guid(const GUID &guid);
    Guid(const Bstr &string);
    Guid(const char *cstring);
    Guid(const char *array,
        const size_t length);
    Guid(const std::string &string);
    Guid(const wchar_t *cstring);
    Guid(const wchar_t *array,
        const size_t length);
    Guid(const std::wstring &string);

    // DATA
    static Guid fromProgid(const std::string &string);
    static Guid fromProgid(const std::wstring &string);
    std::string toProgid();
    static Guid fromClsid(const std::string &string);
    static Guid fromClsid(const std::wstring &string);
    std::string toClsid();
    static Guid fromIid(const std::string &string);
    static Guid fromIid(const std::wstring &string);
    std::string toIid();

    std::string uuid() const;
    std::string define(const std::string &prefix,
        const std::string &name) const;

    friend bool operator==(const Guid &left,
        const Guid &right);
    friend bool operator!=(const Guid &left,
        const Guid &right);
};


}   /* autocom */
