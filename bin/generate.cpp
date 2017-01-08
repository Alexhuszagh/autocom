//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief Query COM interface and generate compile-time interface.
 */

#include "options.hpp"
#include "parse.hpp"
#include "write.hpp"

namespace com = autocom;

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
        exit(EXIT_SUCCESS);
    }

    exit(EXIT_FAILURE);
}
