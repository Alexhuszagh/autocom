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

#include <iostream>

namespace com = autocom;
using namespace com::literals;


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
    std::cout << "Version is " << version << "\n";

    dispatch.method(L"SetCurrentController", 0_I4, 1_I4);

    com::Variant info;
    LONG size = 0;
    dispatch.method(L"GetPrecursorInfoFromScanNum", 3_I4, &info, &size);
    std::cout << "Precursor info size is " << size << "\n";
    com::SafeArray<PrecursorInfo> array(info);
    for (auto it = array.begin(); it < array.begin() + size; ++it) {
        std::cout << "PrecursorInfo(dIsolationMass=" << it->dIsolationMass
                  << ", dMonoIsoMass=" << it->dMonoIsoMass
                  << ", nChargeState=" << it->nChargeState
                  << ", nScanNumber=" << it->nScanNumber
                  << ")\n";
    }

    dispatch.method(L"Close");

    return 0;
}
