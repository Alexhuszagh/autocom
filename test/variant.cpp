//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoComTests
 *  \brief Variant wrapper test suite.
 */

#include "autocom.hpp"

#include <gtest/gtest.h>

namespace com = autocom;


// HELPERS
// -------

template <typename T>
void testSet(com::Variant &variant,
    T &t,
    DWORD vartype)
{
    variant.set(t);
    EXPECT_EQ(variant.vt, vartype);
    variant.set(&t);
    EXPECT_EQ(variant.vt, vartype | VT_BYREF);
}


template <
    typename Safe,
    typename SafePtr,
    DWORD VT
>
void testSetSafe(com::Variant &variant)
{
    typename Safe::type t;
    variant.set(Safe(t));
    EXPECT_EQ(variant.vt, VT);

    variant.set(SafePtr(t));
    EXPECT_EQ(variant.vt, VT | VT_BYREF);
}


#define TEST_SET testSet
#define TEST_SET_SAFE(Safe, Vt) testSetSafe<Safe, Safe##Ptr, Vt>


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


TEST(Variant, SetVariant)
{
    com::Variant variant;

    // nullptr
    variant.set(nullptr);
    EXPECT_EQ(variant.vt, VT_NULL);

    // CHAR
    CHAR c;
    TEST_SET(variant, c, VT_I1);

    // UCHAR
    UCHAR uc;
    TEST_SET(variant, uc, VT_UI1);

    // SHORT
    SHORT shrt;
    TEST_SET(variant, shrt, VT_I2);

    // USHORT
    USHORT ushrt;
    TEST_SET(variant, ushrt, VT_UI2);

    // INT
    INT int_;
    TEST_SET(variant, int_, VT_INT);

    // UINT
    UINT uint;
    TEST_SET(variant, uint, VT_UINT);

    // LONG
    LONG lng;
    TEST_SET(variant, lng, VT_I4);

    // ULONG
    ULONG ulng;
    TEST_SET(variant, ulng, VT_UI4);

    // FLOAT
    FLOAT flt;
    TEST_SET(variant, flt, VT_R4);

    // DOUBLE
    DOUBLE dbl;
    TEST_SET(variant, dbl, VT_R8);

    // LONGLONG
    LONGLONG lnglng;
    TEST_SET(variant, lnglng, VT_I8);

    // ULONGLONG
    ULONGLONG ulnglng;
    TEST_SET(variant, ulnglng, VT_UI8);

    // CURRENCY
    CURRENCY cy;
    TEST_SET(variant, cy, VT_CY);

    // DECIMAL
    DECIMAL dec;
    // cannot pass by-value, not part of Variant union
    variant.set(&dec);
    EXPECT_EQ(variant.vt, VT_DECIMAL | VT_BYREF);

    // BSTR
    BSTR bstr;
    TEST_SET(variant, bstr, VT_BSTR);

    // LITERALS
    variant.set(L"");
    EXPECT_EQ(variant.vt, VT_BSTR);
    variant.set("");
    EXPECT_EQ(variant.vt, VT_BSTR);

    // Bstr
    com::Bstr string;
    TEST_SET(variant, string, VT_BSTR);

    // IUnknown
    IUnknown *unknown;
    TEST_SET(variant, unknown, VT_UNKNOWN);

    // IDispatch
    IDispatch *dispatch;
    TEST_SET(variant, dispatch, VT_DISPATCH);
}


TEST(Variant, SafeSetVariant)
{
    com::Variant variant;

    // nullptr
    variant.set(com::SafeNull(nullptr));
    EXPECT_EQ(variant.vt, VT_NULL);

    // Wrappers
    TEST_SET_SAFE(com::SafeBool, VT_BOOL)(variant);
    TEST_SET_SAFE(com::SafeChar, VT_I1)(variant);
    TEST_SET_SAFE(com::SafeUChar, VT_UI1)(variant);
    TEST_SET_SAFE(com::SafeShort, VT_I2)(variant);
    TEST_SET_SAFE(com::SafeUShort, VT_UI2)(variant);
    TEST_SET_SAFE(com::SafeInt, VT_INT)(variant);
    TEST_SET_SAFE(com::SafeUInt, VT_UINT)(variant);
    TEST_SET_SAFE(com::SafeLong, VT_I4)(variant);
    TEST_SET_SAFE(com::SafeULong, VT_UI4)(variant);
    TEST_SET_SAFE(com::SafeFloat, VT_R4)(variant);
    TEST_SET_SAFE(com::SafeDouble, VT_R8)(variant);
    TEST_SET_SAFE(com::SafeLongLong, VT_I8)(variant);
    TEST_SET_SAFE(com::SafeULongLong, VT_UI8)(variant);
    TEST_SET_SAFE(com::SafeBstr, VT_BSTR)(variant);
    TEST_SET_SAFE(com::SafeCurrency, VT_CY)(variant);
    TEST_SET_SAFE(com::SafeError, VT_ERROR)(variant);
    TEST_SET_SAFE(com::SafeDate, VT_DATE)(variant);
    TEST_SET_SAFE(com::SafeIUnknown, VT_UNKNOWN)(variant);
    TEST_SET_SAFE(com::SafeIDispatch, VT_DISPATCH)(variant);

    DECIMAL dec;
    variant.set(com::SafeDecimalPtr(dec));
    EXPECT_EQ(variant.vt, VT_DECIMAL | VT_BYREF);
}


TEST(Variant, GetVariant)
{
}


TEST(Variant, SafeGetVariant)
{
}
