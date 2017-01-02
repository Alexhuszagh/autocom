//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoComTests
 *  \brief SaffeArray test suite.
 */

#include "autocom.hpp"

#include <gtest/gtest.h>

namespace com = autocom;


struct X { int y; };


// TESTS
// -----


TEST(SafeArrayBound, Methods)
{
    com::SafeArrayBound bound;
    bound.lower() = 0;
    bound.upper() = 5;
    EXPECT_EQ(bound.size(), 5);
}


TEST(SafeArray, Stl)
{
    com::SafeArray<INT> array = {3, 4, 5};
    EXPECT_EQ(array.size(), 3);
    EXPECT_EQ(array.front(), 3);
    EXPECT_EQ(array.back(), 5);
    EXPECT_EQ(array[0], 3);
    EXPECT_EQ(array[1], 4);
    EXPECT_EQ(array[2], 5);

    LONG index = 0;
    EXPECT_EQ(array[&index], 3);

    std::vector<INT> copy(array.begin(), array.end());
    EXPECT_EQ(copy[0], 3);
    EXPECT_EQ(copy[2], 5);
}


TEST(SafeArray, Type)
{
     EXPECT_EQ(com::SafeArray<X>::vt, VT_RECORD);
     EXPECT_EQ(com::SafeArray<INT>::vt, VT_INT);
}
