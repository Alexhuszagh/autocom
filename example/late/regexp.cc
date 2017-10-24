//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief Example to use VBScript Regexp modules.
 */

#include "autocom.hpp"

#include <iostream>

namespace com = autocom;


/** \brief Execute main code block.
 */
int main(int argc, char *argv[])
{
    com::Dispatch dispatch(L"VBScript.RegExp");
    dispatch.put(L"Pattern", L"\\w+");
    dispatch.put(L"IgnoreCase", TRUE);
    dispatch.put(L"Global", TRUE);

    // get matches
    INT index, length;
    com::Bstr text;
    for (auto match: dispatch.iter(L"Execute", L"A(b) c35 d_[x] yyy")) {
        match.get(L"FirstIndex", index);
        match.get(L"Length", length);
        match.get(L"Value", text);
        std::wcout << L"Match found from " << index << "-" << index+length
                   << " and is '" << text << "'\n";
    }

    return 0;
}
