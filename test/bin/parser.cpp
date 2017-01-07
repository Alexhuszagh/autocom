//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup AutoComTests
 *  \brief BSTR wrapper test suite.
 */

#include "parser.hpp"

#include <gtest/gtest.h>

namespace com = autocom;


// TESTS
// -----


TEST(EnumValue, Header)
{
    com::detail::EnumValue value;
    value.name = "VALUE";
    value.value = "1";

    EXPECT_EQ(value.header(), "VALUE = 1");
}


TEST(Parameter, Header)
{
    com::detail::Parameter value;
    value.type = "LONG";
    value.name = "flag";

    EXPECT_EQ(value.header(), "LONG flag");
}


TEST(Variable, Header)
{
    com::detail::Variable value;
    value.type = "LONG";
    value.name = "flag";
    value.value = "1";

    EXPECT_EQ(value.header(), "extern LONG flag");
}


TEST(Property, Header)
{
    // TODO:
}


TEST(Function, Header)
{
    com::detail::Function value;
    value.decorator = "__stdcall";
    value.returns = "void";
    value.name = "Open";
    value.args.resize(1);
    value.args[0].type = "BSTR";
    value.args[0].name = "arg0";

    EXPECT_EQ(value.header(), "__stdcall void Open(BSTR arg0);");
}


TEST(Enum, Header)
{
    com::detail::Enum value;
    value.name = "Enum";
    value.values.resize(2);
    value.values[0].name = "FIRST";
    value.values[0].value = "0";
    value.values[1].name = "SECOND";
    value.values[1].value = "1";

    EXPECT_EQ(value.header(), "enum Enum\r\n{\r\n    FIRST = 0,\r\n    SECOND = 1,\r\n};\r\n");
}


TEST(Record, Forward)
{
    com::detail::Record value;
    value.name = "Record";

    EXPECT_EQ(value.forward(), "struct Record;");
}


TEST(Record, Header)
{
    com::detail::Record value;
    value.name = "Record";
    value.size = sizeof(LONG);
    value.fields.resize(1);
    value.fields[0].type = "LONG";
    value.fields[0].name = "value";

    EXPECT_EQ(value.header(), "struct Record\r\n{\r\n    LONG value;\r\n};\r\nstatic_assert(sizeof(Record) == 4, \"AutoCOM: Invalid struct size.\");\r\n");
}


TEST(Module, Header)
{
    // TODO: no known examples...
}


TEST(Interface, Header)
{
    // TODO: implement
}


TEST(Dispatch, Header)
{
    // TODO: implement
}


TEST(CoClass, Header)
{
    com::detail::CoClass value;
    //value.name =
    // TODO: implement
}


TEST(Alias, Header)
{
    com::detail::Alias value;
    value.name = "Type";
    value.type = "Alias";

    EXPECT_EQ(value.header(), "typedef Type Alias;");
}


TEST(Union, Header)
{
    com::detail::Union value;
    value.name = "Union";
    value.fields.resize(2);
    value.fields[0].type = "LONG";
    value.fields[0].name = "llVal";
    value.fields[1].type = "ULONG";
    value.fields[1].name = "ullVal";

    EXPECT_EQ(value.header(), "union Union\r\n{\r\n    LONG llVal;\r\n    ULONG ullVal;\r\n};\r\n");
}
