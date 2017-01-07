//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup AutoComTests
 *  \brief Encoding converter unittests.
 */

#include "autocom.hpp"

#include <gtest/gtest.h>

namespace com = autocom;

// TESTS
// -----


TEST(EncodingConverters, Wide)
{
    std::string utf8 = {-19, -107, -100, -22, -75, -83, -20, -106, -76};
    auto wide = com::WIDE(utf8);
    EXPECT_EQ(wide.size(), 3);
    EXPECT_EQ(wide[0], 54620);
    EXPECT_EQ(wide[1], 44397);
    EXPECT_EQ(wide[2], 50612);
}
