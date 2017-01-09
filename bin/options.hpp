//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief Validate command-line options.
 */

#pragma once

#include <gflags/gflags.h>


// VALIDATORS
// ----------

/** \brief Validate PROGID for COM object.
 */
bool ValidateProgId(const char *flagname,
    const std::string &id);

/** \brief Validate namespace for COM object.
 */
bool ValidateNamespace(const char *flagname,
    const std::string &ns);
