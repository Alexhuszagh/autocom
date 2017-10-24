//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief Validate command-line options.
 */

#pragma once

#include <gflags/gflags.h>

#include <unordered_map>

// MODES
// -----

enum AutoComMode
{
    AUTOCOM_GENERATE    = 0,
    AUTOCOM_PROGID      = 1,
    AUTOCOM_CLSID       = 2,
};

/** \brief Case-insensitive hash for ASCII.
 */
struct CaseInsensitiveHash
{
    size_t operator()(std::string string) const;
};

/** \brief Validate and identify executable mode.
 */
extern std::unordered_map<std::string, AutoComMode, CaseInsensitiveHash> AutoComModes;

// VALIDATORS
// ----------

/** \brief Validate PROGID for COM object.
 */
bool ValidateProgId(const char *flagname,
    const std::string &id);

/** \brief Validate mode for AutoCOM executable.
 */
bool ValidateMode(const char *flagname,
    const std::string &mode);

/** \brief Validate namespace for COM object.
 */
bool ValidateNamespace(const char *flagname,
    const std::string &ns);
