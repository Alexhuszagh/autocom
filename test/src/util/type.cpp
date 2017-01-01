//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoComTests
 *  \brief Type wrapper test suite.
 */

#include "autocom.hpp"

#include <gtest/gtest.h>

namespace com = autocom;


// HELPERS
// -------


template <
    typename T,
    DWORD VT
>
void testVariantType()
{
    EXPECT_EQ(com::VariantType<T>::vt, VT);
    EXPECT_EQ(com::VariantType<T*>::vt, VT | VT_BYREF);
}


template <
    typename Get,
    typename GetPtr,
    typename Put,
    typename PutPtr,
    DWORD VT
>
void testVariantWrapper()
{
    EXPECT_EQ(com::VariantType<Get>::vt, VT);
    EXPECT_EQ(com::VariantType<GetPtr>::vt, VT | VT_BYREF);
    EXPECT_EQ(com::VariantType<Put>::vt, VT);
    EXPECT_EQ(com::VariantType<PutPtr>::vt, VT | VT_BYREF);
}


#define TEST_VARIANT_TYPE(T, VT) testVariantType<T, VT>
#define TEST_VARIANT_WRAPPER(T, VT) testVariantWrapper<com::Get##T, com::Get##T##Ptr, com::Put##T, com::Put##T##Ptr, VT>


// TESTS
// -----


TEST(PutBool, Conversion)
{
    com::PutBool value(TRUE);
    EXPECT_TRUE(VARIANT_BOOL(value));

    value = FALSE;
    EXPECT_FALSE(VARIANT_BOOL(value));
}


TEST(VariantType, Types)
{
    EXPECT_EQ(com::VariantType<std::nullptr_t>::vt, VT_NULL);
    EXPECT_EQ(com::VariantType<VARIANT*>::vt, VT_VARIANT | VT_BYREF);
    TEST_VARIANT_TYPE(void, VT_VOID)();
    TEST_VARIANT_TYPE(CHAR, VT_I1)();
    TEST_VARIANT_TYPE(UCHAR, VT_UI1)();
    TEST_VARIANT_TYPE(SHORT, VT_I2)();
    TEST_VARIANT_TYPE(USHORT, VT_UI2)();
    TEST_VARIANT_TYPE(INT, VT_INT)();
    TEST_VARIANT_TYPE(UINT, VT_UINT)();
    TEST_VARIANT_TYPE(LONG, VT_I4)();
    TEST_VARIANT_TYPE(ULONG, VT_UI4)();
    TEST_VARIANT_TYPE(LONGLONG, VT_I8)();
    TEST_VARIANT_TYPE(ULONGLONG, VT_UI8)();
    TEST_VARIANT_TYPE(FLOAT, VT_R4)();
    TEST_VARIANT_TYPE(DOUBLE, VT_R8)();
    TEST_VARIANT_TYPE(CURRENCY, VT_CY)();
    TEST_VARIANT_TYPE(BSTR, VT_BSTR)();
    TEST_VARIANT_TYPE(DECIMAL, VT_DECIMAL)();
    TEST_VARIANT_TYPE(IUnknown*, VT_UNKNOWN)();
    TEST_VARIANT_TYPE(IDispatch*, VT_DISPATCH)();
}


TEST(VariantType, Wrappers)
{
    TEST_VARIANT_TYPE(com::Bstr, VT_BSTR)();
    EXPECT_EQ(com::VariantType<com::Variant*>::vt, VT_VARIANT | VT_BYREF);
    EXPECT_EQ(com::VariantType<com::PutVariant>::vt, VT_VARIANT | VT_BYREF);
    EXPECT_EQ(com::VariantType<com::GetVariant>::vt, VT_VARIANT | VT_BYREF);
    EXPECT_EQ(com::VariantType<com::GetDecimalPtr>::vt, VT_DECIMAL | VT_BYREF);
    EXPECT_EQ(com::VariantType<com::PutDecimalPtr>::vt, VT_DECIMAL | VT_BYREF);

    TEST_VARIANT_WRAPPER(Bool, VT_BOOL)();
    TEST_VARIANT_WRAPPER(Char, VT_I1)();
    TEST_VARIANT_WRAPPER(UChar, VT_UI1)();
    TEST_VARIANT_WRAPPER(Short, VT_I2)();
    TEST_VARIANT_WRAPPER(UShort, VT_UI2)();
    TEST_VARIANT_WRAPPER(Int, VT_INT)();
    TEST_VARIANT_WRAPPER(UInt, VT_UINT)();
    TEST_VARIANT_WRAPPER(Long, VT_I4)();
    TEST_VARIANT_WRAPPER(ULong, VT_UI4)();
    TEST_VARIANT_WRAPPER(Float, VT_R4)();
    TEST_VARIANT_WRAPPER(LongLong, VT_I8)();
    TEST_VARIANT_WRAPPER(ULongLong, VT_UI8)();
    TEST_VARIANT_WRAPPER(Double, VT_R8)();
    TEST_VARIANT_WRAPPER(Bstr, VT_BSTR)();
    TEST_VARIANT_WRAPPER(Currency, VT_CY)();
    TEST_VARIANT_WRAPPER(Error, VT_ERROR)();
    TEST_VARIANT_WRAPPER(Date, VT_DATE)();
    TEST_VARIANT_WRAPPER(IUnknown, VT_UNKNOWN)();
    TEST_VARIANT_WRAPPER(IDispatch, VT_DISPATCH)();
}
