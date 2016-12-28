//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoComTests
 *  \brief Variant wrapper test suite.
 */

#include "autocom.hpp"

#include <gtest/gtest.h>

namespace com = autocom;


// TESTS
// -----


TEST(Variant, Variant)
{
    com::Variant variant;
    EXPECT_EQ(variant.vt, VT_EMPTY);

    // set field
    variant.vt = VT_I1;
    EXPECT_EQ(variant.vt, VT_I1);

    // clear
    variant.clear();
    EXPECT_EQ(variant.vt, VT_EMPTY);
}
