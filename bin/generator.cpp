//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief Query COM interface and generate compile-time interface.
 */

#include "parser.hpp"
#include "writer.hpp"

#include <gflags/gflags.h>

namespace com = autocom;


// VALIDATORS
// ----------


/** \brief Validate PROGID for COM object.
 *
 *  Value can either be a PROGID, or a CLSID as a 36-character hex
 *  identifier enclosed in brackets.
 */
static bool ValidateProgId(const char *flagname,
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

// OPTIONS
// -------

DEFINE_string(progid, "", "Program ID or CLSID for COM object");
DEFINE_string(header, "./", "Directory to store generated header.");
DEFINE_validator(progid, &ValidateProgId);

// FUNCTIONS
// ---------


/** \brief Execute main code block.
 */
int main(int argc, char *argv[])
{
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    com::Dispatch dispatch(FLAGS_progid);
    if (dispatch) {
        // parse descriptions
        com::TypeLibDescription description;
        description.parse(dispatch.info().typelib());

        // write to file
        com::Files files;
        writeHeaders(description, FLAGS_header, files);
    }

    return 0;
}
