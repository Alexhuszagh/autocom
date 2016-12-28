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
    DWORD vartype)
{
    T t;
    variant.set(t);
    EXPECT_EQ(variant.vt, vartype);
    variant.set(&t);
    EXPECT_EQ(variant.vt, vartype | VT_BYREF);
}


template <
    typename Type,
    typename TypePtr,
    DWORD VT
>
void testSetWrapper(com::Variant &variant)
{
    variant.set(Type());
    EXPECT_EQ(variant.vt, VT);

    variant.set(TypePtr());
    EXPECT_EQ(variant.vt, VT | VT_BYREF);
}


template <typename T>
void testGet(com::Variant &variant)
{
    T t;
    T *p = &t;
    variant.set(t);
    variant.get(t);

    variant.set(p);
    variant.get(p);
}


template <
    typename GetType,
    typename GetTypePtr,
    typename PutType,
    typename PutTypePtr
>
void testGetWrapper(com::Variant &variant)
{
    typedef typename PutType::type T;
    T t;
    T *p = &t;
    variant.set(PutType(t));
    variant.get(GetType(t));

    variant.set(PutTypePtr(p));
    variant.get(GetTypePtr(p));
}



#define TEST_SET(T) testSet<T>
#define TEST_SET_WRAPPER(Type, Vt) testSetWrapper<Type, Type##Ptr, Vt>
#define TEST_GET(T) testGet<T>
#define TEST_GET_WRAPPER(Type) testGetWrapper<com::Get##Type, com::Get##Type##Ptr, com::Put##Type, com::Put##Type##Ptr>


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

    // DECIMAL
    DECIMAL dec;
    variant.set(&dec);
    EXPECT_EQ(variant.vt, VT_DECIMAL | VT_BYREF);

    // LITERALS
    variant.set(L"");
    EXPECT_EQ(variant.vt, VT_BSTR);
    variant.set("");
    EXPECT_EQ(variant.vt, VT_BSTR);

    TEST_SET(CHAR)(variant, VT_I1);
    TEST_SET(UCHAR)(variant, VT_UI1);
    TEST_SET(SHORT)(variant, VT_I2);
    TEST_SET(USHORT)(variant, VT_UI2);
    TEST_SET(INT)(variant, VT_INT);
    TEST_SET(UINT)(variant, VT_UINT);
    TEST_SET(LONG)(variant, VT_I4);
    TEST_SET(ULONG)(variant, VT_UI4);
    TEST_SET(FLOAT)(variant, VT_R4);
    TEST_SET(DOUBLE)(variant, VT_R8);
    TEST_SET(LONGLONG)(variant, VT_I8);
    TEST_SET(ULONGLONG)(variant, VT_UI8);
    TEST_SET(CURRENCY)(variant, VT_CY);
    TEST_SET(BSTR)(variant, VT_BSTR);
    TEST_SET(com::Bstr)(variant, VT_BSTR);
    TEST_SET(IUnknown*)(variant, VT_UNKNOWN);
    TEST_SET(IDispatch*)(variant, VT_DISPATCH);
}


TEST(Variant, SetWrapperVariant)
{
    com::Variant variant;

    // NULLPTR
    variant.set(com::PutNull(nullptr));
    EXPECT_EQ(variant.vt, VT_NULL);

    // DECIMAL
    DECIMAL dec;
    variant.set(com::PutDecimalPtr(&dec));
    EXPECT_EQ(variant.vt, VT_DECIMAL | VT_BYREF);

    // WRAPPERS
    TEST_SET_WRAPPER(com::PutBool, VT_BOOL)(variant);
    TEST_SET_WRAPPER(com::PutChar, VT_I1)(variant);
    TEST_SET_WRAPPER(com::PutUChar, VT_UI1)(variant);
    TEST_SET_WRAPPER(com::PutShort, VT_I2)(variant);
    TEST_SET_WRAPPER(com::PutUShort, VT_UI2)(variant);
    TEST_SET_WRAPPER(com::PutInt, VT_INT)(variant);
    TEST_SET_WRAPPER(com::PutUInt, VT_UINT)(variant);
    TEST_SET_WRAPPER(com::PutLong, VT_I4)(variant);
    TEST_SET_WRAPPER(com::PutULong, VT_UI4)(variant);
    TEST_SET_WRAPPER(com::PutFloat, VT_R4)(variant);
    TEST_SET_WRAPPER(com::PutDouble, VT_R8)(variant);
    TEST_SET_WRAPPER(com::PutLongLong, VT_I8)(variant);
    TEST_SET_WRAPPER(com::PutULongLong, VT_UI8)(variant);
    TEST_SET_WRAPPER(com::PutBstr, VT_BSTR)(variant);
    TEST_SET_WRAPPER(com::PutCurrency, VT_CY)(variant);
    TEST_SET_WRAPPER(com::PutError, VT_ERROR)(variant);
    TEST_SET_WRAPPER(com::PutDate, VT_DATE)(variant);
    TEST_SET_WRAPPER(com::PutIUnknown, VT_UNKNOWN)(variant);
    TEST_SET_WRAPPER(com::PutIDispatch, VT_DISPATCH)(variant);
}


TEST(Variant, GetVariant)
{
    com::Variant variant;

    // DECIMAL
    DECIMAL dec, *pdec;
    variant.set(&dec);
    variant.get(pdec);

    TEST_GET(CHAR)(variant);
    TEST_GET(UCHAR)(variant);
    TEST_GET(SHORT)(variant);
    TEST_GET(USHORT)(variant);
    TEST_GET(INT)(variant);
    TEST_GET(UINT)(variant);
    TEST_GET(LONG)(variant);
    TEST_GET(ULONG)(variant);
    TEST_GET(FLOAT)(variant);
    TEST_GET(DOUBLE)(variant);
    TEST_GET(LONGLONG)(variant);
    TEST_GET(ULONGLONG)(variant);
    TEST_GET(CURRENCY)(variant);
    TEST_GET(BSTR)(variant);
    TEST_GET(com::Bstr)(variant);
    TEST_GET(IUnknown*)(variant);
    TEST_GET(IDispatch*)(variant);
}


TEST(Variant, GetWrapperVariant)
{
    com::Variant variant;

    // DECIMAL
    DECIMAL dec, *pdec;
    variant.set(com::PutDecimalPtr(&dec));
    variant.get(com::GetDecimalPtr(pdec));

    // WRAPPERS
    TEST_GET_WRAPPER(Bool)(variant);
    TEST_GET_WRAPPER(Char)(variant);
    TEST_GET_WRAPPER(UChar)(variant);
    TEST_GET_WRAPPER(Short)(variant);
    TEST_GET_WRAPPER(UShort)(variant);
    TEST_GET_WRAPPER(Int)(variant);
    TEST_GET_WRAPPER(UInt)(variant);
    TEST_GET_WRAPPER(Long)(variant);
    TEST_GET_WRAPPER(ULong)(variant);
    TEST_GET_WRAPPER(Float)(variant);
    TEST_GET_WRAPPER(Double)(variant);
    TEST_GET_WRAPPER(LongLong)(variant);
    TEST_GET_WRAPPER(ULongLong)(variant);
    TEST_GET_WRAPPER(Bstr)(variant);
    TEST_GET_WRAPPER(Currency)(variant);
    TEST_GET_WRAPPER(Error)(variant);
    TEST_GET_WRAPPER(Date)(variant);
    TEST_GET_WRAPPER(IUnknown)(variant);
    TEST_GET_WRAPPER(IDispatch)(variant);
}
