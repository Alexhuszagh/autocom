//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCom
 *  \brief Example to use Thermo Finnigan's MSFileReader API.
 *
 *  This requires the 2.2.61.0 x86 or x86_64 version of Thermo's
 *  MSFileReader, which can be donwloaded from their site after
 *  registering.
 */

#include "autocom.hpp"

#include <cstdio>

namespace com = autocom;


/** \brief Execute main code block.
 */
int main(int argc, char *argv[])
{
    com::Dispatch dispatch("{1D23188D-53FE-4C25-B032-DC70ACDBDC02}");
    dispatch.method("Open", "Thermo.raw");

    LONG version;
    dispatch.method("GetVersionNumber", &version);
    printf("Version is %d\n", version);

    return 0;
}
