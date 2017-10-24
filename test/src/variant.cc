//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup AutoComTests
 *  \brief Variant wrapper test suite.
 */

#include <autocom.h>
#include <gtest/gtest.h>

namespace com = autocom;


// HELPERS
// -------

template <typename T>
void testSet(com::Variant &variant,
    DWORD vartype,
    T t = T())
{
    variant.set(t);
    EXPECT_EQ(variant.vt, vartype);
    variant.set(&t);
    EXPECT_EQ(variant.vt, vartype | VT_BYREF);
}


template <
    typename PutType,
    typename PutTypePtr,
    typename GetType,
    typename GetTypePtr
>
void testSetWrapper(com::Variant &variant,
    typename PutType::type t = typename PutType::type())
{
    typedef typename PutType::type T;
    T *p = &t;
    variant.set(PutType(t));
    EXPECT_EQ(variant.vt, com::VariantType<PutType>::vt);
    variant.set(GetType(t));
    EXPECT_EQ(variant.vt, com::VariantType<PutType>::vt);

    variant.set(PutTypePtr(p));
    EXPECT_EQ(variant.vt, com::VariantType<PutTypePtr>::vt);
    variant.set(PutTypePtr(p));
    EXPECT_EQ(variant.vt, com::VariantType<PutTypePtr>::vt);
}


template <typename T>
void testGet(com::Variant &variant,
    T t = T())
{
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
#define TEST_SET_WRAPPER(Type) testSetWrapper<com::Put##Type, com::Put##Type##Ptr, com::Get##Type, com::Get##Type##Ptr>
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

    variant = com::Variant(LONG(1));
    EXPECT_EQ(variant.vt, VT_I4);
}


TEST(Variant, Set)
{
    com::Variant variant;

    // nullptr
    variant.set(nullptr);
    EXPECT_EQ(variant.vt, VT_NULL);

    // bool
    variant.set(true);
    EXPECT_EQ(variant.vt, VT_BOOL);

    // DECIMAL
    DECIMAL dec;
    variant.set(&dec);
    EXPECT_EQ(variant.vt, VT_DECIMAL | VT_BYREF);

    // VARIANT
    com::Variant var;
    variant.set(&var);
    EXPECT_EQ(variant.vt, VT_VARIANT | VT_BYREF);

    // CHAR LITERAL
    variant.set('\0');
    EXPECT_EQ(variant.vt, VT_I1);

    // STRING LITERALS
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
    TEST_SET(BSTR)(variant, VT_BSTR, nullptr);
    TEST_SET(com::Bstr)(variant, VT_BSTR);
    TEST_SET(IUnknown*)(variant, VT_UNKNOWN);
    TEST_SET(IDispatch*)(variant, VT_DISPATCH);
}


TEST(Variant, SetString)
{
    // need to make sure we don't get any leaks
    {
        com::Variant variant;
        com::Bstr bstr(L"1");
        EXPECT_TRUE(bstr);
        variant.set(bstr);

        // check empty and null
        EXPECT_FALSE(bstr);
    }
    {
        // move construction, if it is implicitly converted to BSTR,
        // there will be a double-free
        com::Variant variant;
        variant.set(com::Bstr(L"1"));
    }
    {
        // check pointer
        com::Variant variant;
        com::Bstr bstr(L"1");
        variant.set(&bstr);
        EXPECT_TRUE(bstr);
    }
}


TEST(Variant, SetArray)
{
    // need to make sure we don't get any leaks
    {
        com::Variant variant;
        com::SafeArray<INT> array = {1, 2, 3};
        EXPECT_TRUE(array);
        variant.set(array);

        // check empty and null
        EXPECT_FALSE(array);
    }
    {
        // move construction, if it is implicitly converted to SAFEARRAY,
        // there will be a double-free
        com::Variant variant;
        variant.set(com::SafeArray<INT>({1, 2, 3}));
    }
    {
        // check pointer
        com::Variant variant;
        com::SafeArray<INT> array = {1, 2, 3};
        variant.set(&array);
        EXPECT_TRUE(array);
    }
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

    // VARIANT
    com::Variant var;
    variant.set(com::PutVariant(&var));
    EXPECT_EQ(variant.vt, VT_VARIANT | VT_BYREF);

    // WRAPPERS
    TEST_SET_WRAPPER(Bool)(variant);
    TEST_SET_WRAPPER(Char)(variant);
    TEST_SET_WRAPPER(UChar)(variant);
    TEST_SET_WRAPPER(Short)(variant);
    TEST_SET_WRAPPER(UShort)(variant);
    TEST_SET_WRAPPER(Int)(variant);
    TEST_SET_WRAPPER(UInt)(variant);
    TEST_SET_WRAPPER(Long)(variant);
    TEST_SET_WRAPPER(ULong)(variant);
    TEST_SET_WRAPPER(Float)(variant);
    TEST_SET_WRAPPER(Double)(variant);
    TEST_SET_WRAPPER(LongLong)(variant);
    TEST_SET_WRAPPER(ULongLong)(variant);
    TEST_SET_WRAPPER(Bstr)(variant, nullptr);
    TEST_SET_WRAPPER(Currency)(variant);
    TEST_SET_WRAPPER(Error)(variant);
    TEST_SET_WRAPPER(Date)(variant);
    TEST_SET_WRAPPER(IUnknown)(variant, nullptr);
    TEST_SET_WRAPPER(IDispatch)(variant, nullptr);
}


TEST(Variant, GetVariant)
{
    com::Variant variant;

    // TEST VALUE
    LONG value = 20, other;
    variant.set(value);
    variant.get(other);
    EXPECT_EQ(value, other);

    // DECIMAL
    DECIMAL dec, *pdec;
    variant.set(&dec);
    variant.get(pdec);

    // VARIANT
    VARIANT var, *pvar;
    var.vt = VT_INT;
    variant.set(&var);
    variant.get(pvar);
    EXPECT_EQ(pvar->vt, VT_INT);

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
    TEST_GET(BSTR)(variant, nullptr);
    TEST_GET(com::Bstr)(variant);
    TEST_GET(IUnknown*)(variant);
    TEST_GET(IDispatch*)(variant);
}


TEST(Variant, GetWrapperVariant)
{
    com::Variant variant;

    // TEST VALUE
    LONG value = 20, other;
    variant.set(com::PutLong(value));
    variant.get(com::GetLong(other));
    EXPECT_EQ(value, other);

    // DECIMAL
    DECIMAL dec, *pdec;
    variant.set(com::PutDecimalPtr(&dec));
    variant.get(com::GetDecimalPtr(pdec));

    // VARIANT
    VARIANT *ptrvar;
    com::Variant var, *pvar;
    var.vt = VT_INT;
    variant.set(&var);
    variant.get(pvar);
    variant.set(&var);      // null reset, owned, so must "restore"
    variant.get(com::GetVariant(ptrvar));
    EXPECT_EQ(pvar->vt, VT_INT);
    EXPECT_EQ(ptrvar->vt, VT_INT);

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
