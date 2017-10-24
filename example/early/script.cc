//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief Example to use VBScript from C++.
 */

#include "MSScriptControl.hpp"

#include <cassert>

namespace com = autocom;


/** \brief Execute main code block.
 */
int main(int argc, char *argv[])
{
    AutoComScriptControl script;

    com::Bstr command(L"VBScript");
    auto hr = script->Language(command);
    assert(SUCCEEDED(hr));

    hr = script->AllowUI(VARIANT_FALSE);
    assert(SUCCEEDED(hr));

    hr = script->Timeout(-1);
    assert(SUCCEEDED(hr));

    com::Bstr statement(L"var a = 'test'");
    hr = script->ExecuteStatement(statement);
    assert(SUCCEEDED(hr));

    return 0;
}
