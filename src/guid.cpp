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


/** \brief Open GUID from string identifier.
 */
void Guid::open(const std::string &string)
{
    auto wide = WIDE(string);
    if (!wide.empty() && wide.front() == L'{') {
        CLSIDFromString(wide.data(), TO_LPCLSID(&id));
    } else {
        CLSIDFromProgID(wide.data(), TO_LPCLSID(&id));
    }
}


/** \brief Initializer list constructor.
 */
Guid::Guid(const GUID &guid):
    id(guid)
{}


/** \brief Initializer from C-string.
 */
Guid::Guid(const char *cstring)
{
    open(std::string(cstring));
}


/** \brief Initializer from character array.
 */
Guid::Guid(const char *array,
    const size_t length)
{
    open(std::string(array, length));
}


/** \brief Initializer from string identifier.
 */
Guid::Guid(const std::string &string)
{
    open(string);
}


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
    ProgIDFromCLSID(TO_REFCLSID(id), &progid);
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
    StringFromCLSID(TO_REFCLSID(id), &clsid);
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
    IIDFromString(wide.data(), TO_LPIID(&guid.id));

    return guid;
}


/** \brief Get IID from Guid.
 */
std::string Guid::toIid()
{
    LPOLESTR iid;
    StringFromIID(TO_REFIID(id), &iid);
    if (!iid) {
        return "";
    }

    std::string narrow = NARROW(iid);
    CoTaskMemFree(iid);

    return narrow;
}


/** \brief Export GUID to printable representation.
 */
std::string Guid::string() const
{
    // 36 characters + null character
    char *buffer = new char[37];
    const size_t size = sprintf(buffer, "%08lX_%04hX_%04hX_%02hhX%02hhX_%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX", id.Data1, id.Data2, id.Data3, id.Data4[0], id.Data4[1], id.Data4[2], id.Data4[3], id.Data4[4], id.Data4[5], id.Data4[6], id.Data4[7]);
    std::string output(buffer, size);
    delete[] buffer;

    return output;
}


/** \brief Equality operator.
 */
bool operator==(const Guid &left,
    const Guid &right)
{
    return left.id == right.id;
}


/** \brief Inequality operator.
 */
bool operator!=(const Guid &left,
    const Guid &right)
{
    return !operator==(left, right);
}


}   /* autocom */
