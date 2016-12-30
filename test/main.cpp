//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoComTests
 *  \brief AutoCom test runner.
 */

#include <gtest/gtest.h>


// SUITE
// -----


/** \brief Execute test suite.
 */
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
