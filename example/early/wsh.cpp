//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief Example to use Windows Shell (WSH) from C++.
 */

#include "IWshRuntimeLibrary.hpp"

#include <cassert>

namespace com = autocom;


/** \brief Execute main code block.
 */
int main(int argc, char *argv[])
{
    AutoComWshShell wsh;
    com::Bstr command(L"notepad.exe");
    com::Variant style(SHORT(1)), wait;
    ULONG code;
    auto hr = wsh->Run(command.data(), &style, &wait, &code);
    assert(SUCCEEDED(hr));

    return 0;
}
