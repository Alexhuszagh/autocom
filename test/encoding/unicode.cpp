//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoComTests
 *  \brief Unicode conversion unittests.
 */

#include "autocom.hpp"

#include <gtest/gtest.h>

namespace com = autocom;


// TESTS
// -----


TEST(Unicode, TestUtf)
{
    std::string utf8 = {-19, -107, -100, -22, -75, -83, -20, -106, -76};
    std::string utf16 = {92, -43, 109, -83, -76, -59};
    std::string utf32 = {92, -43, 0, 0, 109, -83, 0, 0, -76, -59, 0, 0};

    // FROM UTF8
    EXPECT_EQ(utf16, com::UTF8_TO_UTF16(utf8));
    EXPECT_EQ(utf32, com::UTF8_TO_UTF32(utf8));

    // FROM UTF16
    EXPECT_EQ(utf8, com::UTF16_TO_UTF8(utf16));
    EXPECT_EQ(utf32, com::UTF16_TO_UTF32(utf16));

    // FROM UTF32
    EXPECT_EQ(utf8, com::UTF32_TO_UTF8(utf32));
    EXPECT_EQ(utf16, com::UTF32_TO_UTF16(utf32));
}
