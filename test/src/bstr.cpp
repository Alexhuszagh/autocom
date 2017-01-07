//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup AutoComTests
 *  \brief BSTR wrapper test suite.
 */

#include "autocom.hpp"

#include <gtest/gtest.h>

namespace com = autocom;


// TESTS
// -----


TEST(Bstr, Constructors)
{
    com::Bstr wide(L"data");
    com::Bstr narrow(L"data");

    EXPECT_EQ(wide, narrow);
}


TEST(Bstr, Iterators)
{
    com::Bstr bstr(L"data");
    EXPECT_EQ(*bstr.begin(), L'd');
    EXPECT_EQ(*bstr.rbegin(), L'a');
}


TEST(Bstr, Capacity)
{
    com::Bstr bstr(L"data");
    EXPECT_EQ(bstr.size(), 4);
    EXPECT_EQ(bstr.length(), 4);
    EXPECT_FALSE(bstr.empty());

    bstr.clear();
    EXPECT_EQ(bstr.size(), 0);
    EXPECT_TRUE(bstr.empty());
}


TEST(Bstr, ElementAccess)
{
    com::Bstr bstr(L"data");
    EXPECT_EQ(bstr[1], L'a');

    bstr[1] = L'b';
    EXPECT_EQ(bstr[1], L'b');
}


TEST(Bstr, Modifiers)
{
    com::Bstr bstr(L"data");
    auto copy = bstr.copy();
    SysFreeString(copy);

    bstr.push_back(L'b');
    EXPECT_EQ(bstr.size(), 5);
    EXPECT_EQ(bstr.back(), L'b');
}


TEST(Bstr, OPERATORS)
{
    com::Bstr empty;
    EXPECT_EQ(empty, empty);

    com::Bstr bstr(L"data");
    com::Bstr copy(bstr);
    EXPECT_NE(empty, bstr);
    EXPECT_EQ(bstr, copy);
}
