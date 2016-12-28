//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoComTests
 *  \brief Type wrapper test suite.
 */

#include "autocom.hpp"

#include <gtest/gtest.h>

namespace com = autocom;

// TESTS
// -----


TEST(PutBool, Conversion)
{
    com::PutBool value(TRUE);
    EXPECT_TRUE(VARIANT_BOOL(value));

    value = FALSE;
    EXPECT_FALSE(VARIANT_BOOL(value));
}
