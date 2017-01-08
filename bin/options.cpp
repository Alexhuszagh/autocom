//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief Validate command-line options.
 */

#include "options.hpp"


// VALIDATORS
// ----------


/** Value can either be a PROGID, or a CLSID as a 36-character hex
 *  identifier enclosed in brackets.
 */
bool ValidateProgId(const char *flagname,
    const std::string &id)
{
    if (id.empty()) {
    } else if (id.front() == '{' || id.back() == '}') {
        if (id.front() == '{' && id.back() == '}' && id.size() == 38) {
            return true;
        }
    } else {
        return true;
    }

    printf("Invalid value for --%s: %s\n", flagname, id.data());
    return false;
}
