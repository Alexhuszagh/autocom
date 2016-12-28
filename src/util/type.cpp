//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCom
 *  \brief Strong type wrappers for WinAPI data types.
 */

#include "autocom.hpp"


namespace autocom
{
// OBJECTS
// -------


/** \brief Initialize string from UTF-8 narrow string.
 */
SafeBstr::SafeBstr(const std::string &string):
    string(WIDE(string))
{}


/** \brief Initialize string from UTF-16 wide string.
 */
SafeBstr::SafeBstr(const std::wstring &string):
    string(string)
{}


/** \brief Convert wrapper to BSTR.
 */
SafeBstr::operator BSTR() const
{
    return SysAllocString(string.data());
}


/** \brief Initialize from pointer to BSTR.
 */
SafeBstrRef::SafeBstrRef(BSTR *&string):
    string(string)
{}


SafeBstrRef::SafeBstrRef(BSTR &string):
    string(&string)
{}


/** \brief Convert wrapper to BSTR reference.
 */
SafeBstrRef::operator BstrPtr() const
{
    return string;
}


}   /* autocom */
