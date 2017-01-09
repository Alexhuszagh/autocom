//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief Example to use Thermo Finnigan's MSFileReader API.
 *
 *  This example uses the early-binding interface with the Thermo
 *  MSFileReader API to extract scans from a proteolytic experiment.
 *
 *  This requires the 2.2.61.0 x86 or x86_64 version of Thermo's
 *  MSFileReader, which can be donwloaded from their site after
 *  registering.
 */

#include "MSFileReaderLib.hpp"

#include <cassert>
#include <iostream>

namespace com = autocom;


/** \brief Execute main code block.
 */
int main(int argc, char *argv[])
{
    com::ComObject<MSFileReader_XRawfile, IXRawfile5> raw;
    com::Bstr file(L"Thermo.raw");
    auto hr = raw->Open(file.string);
    assert(SUCCEEDED(hr));

    LONG version = 0;
    hr = raw->GetVersionNumber(&version);
    assert(SUCCEEDED(hr));
    std::cout << "Version is " << version << std::endl;

    hr = raw->SetCurrentController(0, 1);
    assert(SUCCEEDED(hr));

    com::Variant info;
    LONG size = 0;
    hr = raw->GetPrecursorInfoFromScanNum(3, &info, &size);
    assert(SUCCEEDED(hr));
    std::cout << "Precursor info size is " << size << "\n";
    com::SafeArray<MS_PrecursorInfo> array(info);
    for (auto it = array.begin(); it < array.begin() + size; ++it) {
        std::cout << "MS_PrecursorInfo(dMonoIsoMZ=" << it->dMonoIsoMZ
                  << ", dIsolationMZ=" << it->dIsolationMZ
                  << ", nChargeState=" << it->nChargeState
                  << ", nScanNumber=" << it->nScanNumber
                  << ")\n";
    }

    raw->Close();

    return 0;
}
