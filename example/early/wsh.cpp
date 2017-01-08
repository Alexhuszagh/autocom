//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief Example to use Windows Shell (WSH) from C++.
 */

#include "IWshRuntimeLibrary.hpp"

#include <cassert>

namespace com = autocom;
namespace sig = signatures;
using namespace com::literals;

// Cross-DLL types
typedef sig::IWshShell_NS::Run_0_Arg3 CodePtr;
typedef typename com::DereferenceType<CodePtr>::type CodeType;


/** \brief Execute main code block.
 */
int main(int argc, char *argv[])
{
    AutoComWshShell wsh;
    com::Bstr command(L"notepad.exe");
    com::Variant style(1_I2), wait;
    CodeType code;
    auto hr = wsh->Run(command.data(), &style, &wait, &code);
    assert(SUCCEEDED(hr));

    return 0;
}
