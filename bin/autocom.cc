//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief AutoCOM executable.
 *
 *  Modes:
 *Query COM interface and generate compile-time interface
 */

#include "options.h"
#include "parse.h"
#include "write.h"

namespace com = autocom;

// OPTIONS
// -------

DEFINE_string(progid, "", "Program ID or CLSID for COM object");
DEFINE_string(ns, "", "Namespace to store COM definitions.");
DEFINE_string(header, "./", "Directory to store generated header.");
DEFINE_string(mode, "generate", "Enumerated modes for AutoCOM, ['generate', 'progid', 'clsid']");
DEFINE_validator(progid, &ValidateProgId);
DEFINE_validator(ns, &ValidateNamespace);
DEFINE_validator(mode, &ValidateMode);

// FUNCTIONS
// ---------

/** \brief Generate C++ headers from interface.
 */
void generate(com::Dispatch &dispatch)
{
    // parse descriptions
    com::TypeLibDescription description;
    description.parse(dispatch.info().typelib());

    // write to file
    com::Files files;
    writeHeaders(description, FLAGS_ns, FLAGS_header, files);
}


/** \brief Get preferred CLSID.
 */
void getCLSID(com::Dispatch &dispatch)
{
    auto attr = dispatch.info().typelib().attr();
    printf("%s", attr.guid().uuid().data());
}


/** \brief Get preferred ProgID
 */
void getProgID(com::Dispatch &dispatch)
{
    auto tlib = dispatch.info().typelib();
    printf("%s", tlib.documentation(-1).name.data());
}


/** \brief Execute main code block.
 */
int main(int argc, char *argv[])
{
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    com::Dispatch dispatch(FLAGS_progid);
    if (dispatch) {
        switch (AutoComModes[FLAGS_mode]) {
            case AUTOCOM_GENERATE:
                generate(dispatch);
                break;
            case AUTOCOM_PROGID:
                getProgID(dispatch);
                break;
            case AUTOCOM_CLSID:
                getCLSID(dispatch);
                break;
            default:
                throw std::invalid_argument("Unrecognized option.");
        }
        exit(EXIT_SUCCESS);
    }

    exit(EXIT_FAILURE);
}
