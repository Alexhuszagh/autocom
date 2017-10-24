//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup AutoComTests
 *  \brief Dispatch parameters test suite.
 */

#include <autocom.h>
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
    EXPECT_EQ(dispparams.args().size(), 0);
}


TEST(DispParams, SetPrimitive)
{
    com::DispParams dp;

    // generic overloads -- BSTR must be null initialized
    LONG version;
    BSTR value = nullptr;
    dp.setArgs(version, value);
    dp.setArgs(&version, value);
    dp.setArgs(version, &value);
    dp.setArgs(&version, &value);
    EXPECT_EQ(dp.args().size(), 2);

    dp.setArgs();
    EXPECT_EQ(dp.args().size(), 0);

    // safe overloads
    BOOL boolean = FALSE;
    INT integer;

    dp.setArgs(com::PutBool(boolean), com::PutInt(integer));
    EXPECT_EQ(dp.params()->rgvarg[0].vt, 22);
    EXPECT_EQ(dp.params()->rgvarg[1].vt, 11);

    dp.setArgs(com::PutBool(boolean));
    EXPECT_EQ(dp.params()->rgvarg[0].vt, 11);
}


TEST(DispParams, SetString)
{
    com::DispParams dp;
    dp.setArgs(L"wide");
    EXPECT_EQ(dp.params()->rgvarg[0].vt, 8);

    dp.setArgs("narrow");
    EXPECT_EQ(dp.params()->rgvarg[0].vt, 8);

    dp.setArgs(com::PutBstr());
    EXPECT_EQ(dp.params()->rgvarg[0].vt, 8);
}
