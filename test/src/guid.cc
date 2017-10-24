//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup AutoComTests
 *  \brief GUID test suite.
 */

#include <autocom.h>
#include <gtest/gtest.h>


// TESTS
// -----


TEST(GuidTest, Constructor)
{
    std::string progid = "VBScript.RegExp";
    auto guid = autocom::Guid::fromProgid(progid);
    EXPECT_EQ(progid, guid.toProgid());

    std::string clsid = "{1D23188D-53FE-4C25-B032-DC70ACDBDC02}";
    guid = autocom::Guid::fromClsid(clsid);
    EXPECT_EQ(clsid, guid.toClsid());

    std::string iid = "{11B488A0-69B1-41FC-A660-FE8DF2A31F5B}";
    guid = autocom::Guid::fromIid(iid);
    EXPECT_EQ(iid, guid.toIid());
}
