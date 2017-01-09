//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief Validate command-line options.
 */

#include "options.hpp"

#include <regex>

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
        std::regex validator("\\{[a-fA-F0-9]{8}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{12}\\}");
        if (std::regex_match(id, validator)) {
            return true;
        }
    } else {
        // ProgIDs must be 39 characters or less, and cannot have any
        // punctuation, except periods, and cannot start with a digit
        std::regex validator("[A-Za-z][A-Za-z.0-9]{0,38}");
        if (std::regex_match(id, validator)) {
            return true;
        }
    }

    printf("Invalid value for --%s: %s\n", flagname, id.data());
    return false;
}


/** \brief Validate namespace for COM object.
 */
bool ValidateNamespace(const char *flagname,
    const std::string &ns)
{
    std::regex validator("[A-Za-z_]\\w*");
    if (ns.empty() || std::regex_match(ns, validator)) {
        return true;
    }

    printf("Invalid value for --%s: %s\n", flagname, ns.data());
    return false;
}
