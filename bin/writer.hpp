//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief Write class attributes into C++ interface.
 */

#pragma once

#include "parser.hpp"

#include <string>
#include <vector>


namespace autocom
{
// OBJECTS
// -------


/** \brief Documents written to file.
 */
struct Files
{
    std::vector<std::string> headers;
};

// FUNCTIONS
// ---------


/** \brief Write C++ header file from file description.
 */
void writeHeaders(TypeLibDescription &tlib,
    std::string &directory,
    Files &files);

}   /* autocom */
