//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup AutoCOM
 *  \brief Character set conversion utilities.
 */

#include <string>


namespace autocom
{
// FUNCTIONS
// ---------

/** \brief Convert UTF-8 to wide UTF-16.
 */
std::wstring WIDE(const std::string &narrow);


/** \brief Convert UTF-16 to narrow UTF-8.
 */
std::string NARROW(const std::wstring &wide);


}   /* autocom */
