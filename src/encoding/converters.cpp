//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup AutoCom
 *  \brief Character set conversion utilities.
 */

#include "autocom.hpp"


namespace autocom
{
// FUNCTIONS
// ---------

/** \brief Convert UTF-8 to wide UTF-16.
 */
std::wstring WIDE(const std::string &narrow)
{
    auto utf16 = UTF8_TO_UTF16(narrow);
    auto *data = reinterpret_cast<const wchar_t*>(utf16.data());
    const size_t size = utf16.size() / sizeof(wchar_t);

    return std::wstring(data, size);
}


/** \brief Convert UTF-16 to narrow UTF-8.
 */
std::string NARROW(const std::wstring &wide)
{
    auto *data = reinterpret_cast<const char*>(wide.data());
    const size_t size = wide.size() * sizeof(wchar_t);
    std::string utf16(data, size);

    return UTF16_TO_UTF8(utf16);
}


}   /* autocom */
