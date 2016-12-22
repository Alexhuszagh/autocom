//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoComTests
 *  \brief Dispatch parameters test suite.
 */

#include "autocom.hpp"

#include <gtest/gtest.h>

#include <vector>           // TODO: remove


// TESTS
// -----


//TEST(TODORemove, Constructor)
//{
//    std::vector<char> vector = {'c', 'd', 'e'};
//    autocom::test(vector);
//}


TEST(Dispatch, Flags)
{
    auto flags = autocom::Dispatch::METHOD;
    EXPECT_TRUE(!!(flags & autocom::Dispatch::METHOD));
    //autocom::DispParams copy(dispparams);
}


TEST(DispParams, Constructor)
{
    autocom::DispParams dispparams;
    autocom::DispParams copy(dispparams);
}


TEST(DispParams, SetArgs)
{
    autocom::DispParams dispparams;
    long version;
    BSTR value;
    dispparams.setArgs(&version, value);
}
