//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCom
 *  \brief Example to use VBScript Regexp modules.
 */

#include "autocom.hpp"

#include <iostream>

namespace com = autocom;


/** \brief Execute main code block.
 */
int main(int argc, char *argv[])
{
    com::Dispatch dispatch("VBScript.RegExp");
    dispatch.put("Pattern", L"is.");
    dispatch.put("IgnoreCase", TRUE);
    dispatch.put("Global", TRUE);

    // get matches
    INT index, length;
    com::Bstr text;
    for (auto match: dispatch.iter("Execute", L"IS1 is2 IS3 is4")) {
        match.get("FirstIndex", &index);
        match.get("Length", &length);
        match.get("Value", &text);
        printf("Match found from %d-%d and is '%S'\n", index, index+length, text.string);
    }

    return 0;
}
