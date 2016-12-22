//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCom
 *  \brief COM GUID utilities.
 */

#include "autocom.hpp"


namespace autocom
{
// HELPERS
// -------


/** \brief Convert GUID to REFCLSID (CLSID&).
 */
auto TO_REFCLSID(GUID &guid)
    -> REFCLSID
{
    return reinterpret_cast<REFCLSID>(guid);
}


/** \brief Convert GUID to LIPCLSID (CLSID*).
 */
auto TO_LPCLSID(GUID *guid)
    -> LPCLSID
{
    return reinterpret_cast<LPCLSID>(guid);
}


/** \brief Convert GUID to REFIID (IID&).
 */
auto TO_REFIID(GUID &guid)
    -> REFIID
{
    return reinterpret_cast<REFIID>(guid);
}


/** \brief Convert GUID to LPIID (IID*).
 */
auto TO_LPIID(GUID *guid)
    -> LPIID
{
    return reinterpret_cast<LPIID>(guid);
}

// OBJECTS
// -------


/** \brief Null constructor.
 */
Guid::Guid()
{}


/** \brief Initializer list constructor.
 */
Guid::Guid(const GUID &guid):
    guid(guid)
{}


/** \brief Initializer from identifier.
 */
Guid::Guid(const std::string &string)
{
    auto wide = WIDE(string);
    if (!wide.empty() && wide.front() == L'{') {
        CLSIDFromString(wide.data(), TO_LPCLSID(&guid));
    } else {
        CLSIDFromProgID(wide.data(), TO_LPCLSID(&guid));
    }
}


/** \brief Copy constructor.
 */
Guid::Guid(const Guid &other):
    guid(other.guid)
{}


/** \brief Destructor.
 */
Guid::~Guid()
{}


/** \brief Construct Guid from progid.
 */
Guid Guid::fromProgid(const std::string &string)
{
    return Guid(string);
}


/** \brief Get progid from Guid.
 */
std::string Guid::toProgid()
{
    LPOLESTR progid;
    ProgIDFromCLSID(TO_REFCLSID(guid), &progid);
    if (!progid) {
        return "";
    }

    std::string narrow = NARROW(progid);
    CoTaskMemFree(progid);

    return narrow;
}


/** \brief Construct Guid from clsid.
 */
Guid Guid::fromClsid(const std::string &string)
{
    return Guid(string);
}


/** \brief Get clsid from Guid.
 */
std::string Guid::toClsid()
{
    LPOLESTR clsid;
    StringFromCLSID(TO_REFCLSID(guid), &clsid);
    if (!clsid) {
        return "";
    }

    std::string narrow = NARROW(clsid);
    CoTaskMemFree(clsid);

    return narrow;
}


/** \brief Construct Guid from IID.
 */
Guid Guid::fromIid(const std::string &string)
{
    Guid guid;
    auto wide = WIDE(string);
    IIDFromString(wide.data(), TO_LPIID(&guid.guid));

    return guid;
}


/** \brief Get IID from Guid.
 */
std::string Guid::toIid()
{
    LPOLESTR iid;
    StringFromIID(TO_REFIID(guid), &iid);
    if (!iid) {
        return "";
    }

    std::string narrow = NARROW(iid);
    CoTaskMemFree(iid);

    return narrow;
}


/** \brief Equality operator.
 */
bool operator==(const Guid &left,
    const Guid &right)
{
    return left.guid == right.guid;
}


/** \brief Inequality operator.
 */
bool operator!=(const Guid &left,
    const Guid &right)
{
    return !operator==(left, right);
}


}   /* autocom */
