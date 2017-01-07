//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief Example to use Thermo Finnigan's MSFileReader API.
 *
 *  This example uses the late-binding interface with the Thermo
 *  MSFileReader API to extract scans from a proteolytic experiment.
 *
 *  This requires the 2.2.61.0 x86 or x86_64 version of Thermo's
 *  MSFileReader, which can be donwloaded from their site after
 *  registering.
 */

#include "autocom.hpp"

#include <cstdio>

namespace com = autocom;


struct PrecursorInfo
{
    double dIsolationMass;
    double dMonoIsoMass;
    long nChargeState;
    long nScanNumber;
};



/** \brief Execute main code block.
 */
int main(int argc, char *argv[])
{
    com::Dispatch dispatch(L"{1D23188D-53FE-4C25-B032-DC70ACDBDC02}");
    dispatch.method(L"Open", L"Thermo.raw");

    LONG version;
    dispatch.method(L"GetVersionNumber", &version);
    printf("Version is %d\n", version);

    dispatch.method(L"SetCurrentController", LONG(0), LONG(1));

    com::Variant info;
    LONG size = 0;
    dispatch.method(L"GetPrecursorInfoFromScanNum", LONG(3), &info, &size);
    printf("Precursor info size is %d\n", size);
    com::SafeArray<PrecursorInfo> array(info);
    for (auto it = array.begin(); it < array.begin() + size; ++it) {
        printf("PrecursorInfo(dIsolationMass=%f, dMonoIsoMass=%f, nChargeState=%d, nScanNumber=%d)\n", it->dIsolationMass, it->dMonoIsoMass, it->nChargeState, it->nScanNumber);
    }

    dispatch.method(L"Close");

    return 0;
}
