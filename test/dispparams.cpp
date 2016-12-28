//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoComTests
 *  \brief Dispatch parameters test suite.
 */

#include "autocom.hpp"

#include <gtest/gtest.h>

namespace com = autocom;


// TESTS
// -----


TEST(DispatchFlags, Flags)
{
    auto flags = com::DispatchFlags::METHOD;
    EXPECT_TRUE(!!(flags & com::DispatchFlags::METHOD));
}


TEST(DispParams, Constructor)
{
    com::DispParams dispparams;
    com::DispParams copy(dispparams);
}


TEST(DispParams, SetPrimitive)
{
    com::DispParams dp;

    // generic overloads
    LONG version;
    BSTR value;
    dp.setArgs(version, value);
    dp.setArgs(&version, value);
    dp.setArgs(version, &value);
    dp.setArgs(&version, &value);

    // safe overloads
    BOOL boolean;
    INT integer;

    // this could or could not be safe, depending on the compiler
    dp.setArgs(com::SafeBool(boolean), com::SafeInt(integer));
    EXPECT_EQ(dp.params()->rgvarg[0].vt, 22);
    EXPECT_EQ(dp.params()->rgvarg[1].vt, 11);

    dp.setArgs(com::SafeBool(boolean));
    EXPECT_EQ(dp.params()->rgvarg[0].vt, 11);
}


TEST(DispParams, SetString)
{
    com::DispParams dp;
    dp.setArgs(L"wide");
    EXPECT_EQ(dp.params()->rgvarg[0].vt, 8);

    dp.setArgs("narrow");
    EXPECT_EQ(dp.params()->rgvarg[0].vt, 8);

    //dp.setArgs(com::SafeBstr("narrow"));
    //EXPECT_EQ(dp.params()->rgvarg[0].vt, 8);
}
