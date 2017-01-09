//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup AutoComTests
 *  \brief Test C++ type aliases.
 */

#include "autocom.hpp"

#include <gtest/gtest.h>

namespace com = autocom;
using namespace com::literals;
using namespace com::wrappers;


#define TEST_ALIAS3(T, Name, rvalue)                                    \
    {                                                                   \
        typedef T U;                                                    \
        typedef U& R;                                                   \
        U lvalue = rvalue;                                              \
        U r1(P_##Name(U(rvalue)));                                      \
        U r2(W_##Name(U(rvalue)));                                      \
        R l1(G_##Name(lvalue));                                         \
        R l2(W_##Name(lvalue));                                         \
    }


#define TEST_ALIAS2(T, Name)                                            \
    {                                                                   \
        typedef T U;                                                    \
        TEST_ALIAS3(T, Name, U())                                       \
    }

#define TEST_ALIAS1(T)                                                  \
    TEST_ALIAS2(T, T)

#define ID(x) x
#define GET_MACRO(_1,_2,_3,NAME,...) NAME
#define TEST_ALIAS(...) ID(GET_MACRO(__VA_ARGS__, TEST_ALIAS3, TEST_ALIAS2, TEST_ALIAS1)(__VA_ARGS__))


// TESTS
// -----


TEST(LiteralTypes, Vartype)
{
    com::Variant variant;
    variant.set(0_BOOL);
    EXPECT_EQ(variant.vt, VT_BOOL);

    variant.set('\0'_I1);
    variant.set(0_I1);
    EXPECT_EQ(variant.vt, VT_I1);

    variant.set('\0'_UI1);
    variant.set(0_UI1);
    EXPECT_EQ(variant.vt, VT_UI1);

    variant.set(0_I2);
    EXPECT_EQ(variant.vt, VT_I2);

    variant.set(0_UI2);
    EXPECT_EQ(variant.vt, VT_UI2);

    variant.set(0_INT);
    EXPECT_EQ(variant.vt, VT_INT);

    variant.set(0_UINT);
    EXPECT_EQ(variant.vt, VT_UINT);

    variant.set(0_I4);
    EXPECT_EQ(variant.vt, VT_I4);

    variant.set(0_UI4);
    EXPECT_EQ(variant.vt, VT_UI4);

    variant.set(0_I8);
    EXPECT_EQ(variant.vt, VT_I8);

    variant.set(0_UI8);
    EXPECT_EQ(variant.vt, VT_UI8);

    variant.set(0_R4);
    variant.set(0.0_R4);
    EXPECT_EQ(variant.vt, VT_R4);

    variant.set(0_R8);
    variant.set(0.0_R8);
    EXPECT_EQ(variant.vt, VT_R8);

    variant.set(0_DATE);
    variant.set(0.0_DATE);
    EXPECT_EQ(variant.vt, VT_DATE);

    variant.set(0_ERROR);
    EXPECT_EQ(variant.vt, VT_ERROR);
}


TEST(Aliases, Vartype)
{
    TEST_ALIAS(BSTR);
    TEST_ALIAS(CHAR, I1);
    TEST_ALIAS(UCHAR, UI1);
    TEST_ALIAS(SHORT, I2);
    TEST_ALIAS(USHORT, UI2);
    TEST_ALIAS(INT, INT);
    TEST_ALIAS(UINT, UINT);
    TEST_ALIAS(LONG, I4);
    TEST_ALIAS(ULONG, UI4);
    TEST_ALIAS(LONGLONG, I8);
    TEST_ALIAS(ULONGLONG, UI8);
    TEST_ALIAS(FLOAT, R4);
    TEST_ALIAS(DOUBLE, R8);
    TEST_ALIAS(VARIANT_BOOL, BOOL);
    TEST_ALIAS(DATE, DATE);
    TEST_ALIAS(SCODE, ERROR);
    TEST_ALIAS(CURRENCY, CY);
    TEST_ALIAS(SAFEARRAY*, ARRAY);
    TEST_ALIAS(VARIANT*, VARIANT);
    TEST_ALIAS(IUnknown*, UNKNOWN);
    TEST_ALIAS(IDispatch*, DISPATCH);
    TEST_ALIAS(DECIMAL, DECIMAL);
}
