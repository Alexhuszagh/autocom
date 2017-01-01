//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoComTests
 *  \brief SaffeArray test suite.
 */

#include "autocom.hpp"

#include <gtest/gtest.h>

namespace com = autocom;


// TESTS
// -----


TEST(SafeArrayBound, Methods)
{
    com::SafeArrayBound bound;
    bound.lower() = 0;
    bound.upper() = 5;
    EXPECT_EQ(bound.size(), 5);
}


TEST(SafeArray, Methods)
{
    //com::SafeArray array;
    //array.create(VT_INT, 3, );
}
