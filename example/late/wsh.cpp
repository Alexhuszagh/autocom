//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief Example to use Windows Shell (WSH) from C++.
 */

#include "autocom.hpp"

#include <cassert>
#include <cstdio>

namespace com = autocom;


/** \brief Execute main code block.
 */
int main(int argc, char *argv[])
{
    com::Dispatch dispatch(L"WScript.Shell.1");

    com::Variant style(SHORT(1)), wait;
    assert(dispatch.method(L"Run", L"notepad.exe", &style, &wait));

    return 0;
}
