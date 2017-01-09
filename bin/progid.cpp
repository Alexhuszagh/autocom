//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief Query COM interface and get preferred ProgID.
 */

#include <autocom.hpp>
#include "options.hpp"

namespace com = autocom;

// OPTIONS
// -------

DEFINE_string(progid, "", "Program ID or CLSID for COM object");
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
        auto tlib = dispatch.info().typelib();
        printf("%s", tlib.documentation(-1).name.data());
        exit(EXIT_SUCCESS);
    }

    exit(EXIT_FAILURE);
}
