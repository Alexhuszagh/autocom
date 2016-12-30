//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCom
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
    std::vector<std::string> sources;
};

// FUNCTIONS
// ---------


/** \brief Write C++ header file from file description.
 */
void writeHeaders(TypeLibDescription &tlib,
    std::string &directory,
    Files &files);

/** \brief Write C++ source file from file description.
 */
void writeSources(TypeLibDescription &tlib,
    std::string &directory,
    Files &files);


}   /* autocom */
