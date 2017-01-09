
Late-Binding AutoCOM
====================

AutoCOM's late-binding interface uses thread-local reference-counting, C++ variadic templates, and perfect-forwarding to simplify dynamic method dispatch via `IDispatch::Invoke`.

**Table of Contents**

- [Design](#desgin)
  - [IDispatch Wrapper](#idispatch-wrapper)
  - [Method Dispatch](#dispparams-generation)
  - [Value Enumeration](#value-enumeration)
- [Types](#types)
  - [Literals](#literals)
  - [Strong-Typing](#strong-typing)
  - [Type Wrappers](#type-wrappers)
- [Examples](#examples)

## Design

The interface implements a wrapper around an `IDispatch` object, simplifying interface construction, method dispatching, and enumerating over variant collections.

### IDispatch Wrapper

`Dispatch` wraps an `IDispatch` resource, and overloads the constructor so the class can be initialized from a `GUID` object, or a string representing a CLSID or ProgID.

```cpp
{
    // Calls CoInitializeEx before the COM object is created, 
    // internationally gets the CLSID from the ProgID, and
    // creates the IDispatch object.
    autocom::Dispatch dispatch(L"WScript.Shell.1");

// Upon object destruction, `IUnknown::Release` is called, freeing
// the object. Since the COM object is the last existing COM instance,
// the destructor also calls CoUninitialize.
}
```

### Method Dispatch

`Dispatch` implements the helper methods `get`, `put`, `putref`, and `method`, which take a function name and arguments for the COM method, returning whether the COM method succeeded. The `VARTYPE`s  of the arguments are deduced using C++'s strong-typing (see [Types](#types) for more information).

```cpp
// create our arguments
autocom::Variant style, wait;

// Internally, calls GetIDsOfNames() to get the method ID for "Run",
// and constructs DISPPARAMS with 3 arguments of type `VT_BSTR`,
// `VT_VARIANT` and `VT_VARIANT`.
bool status = dispatch.method(L"Run", L"notepad.exe", &style, &wait);
```

To return a variant rather than the call status from the COM method, use the `getV`, `putV`, `putrefV`, or `methodV` analogues instead.

### Value Enumeration

COM methods can return collections of variants through the IEnumVariant interface, representing variable-length, heterogeneous data. The `Dispatch` helper method `iter` wraps the IEnumVariant interface using STL iterators, simplifying value enumeration with auto-ranges.

```cpp
INT index;

// Call a `DISPATCH_PROPERTYGET` method to get the IDispatch object
// for the iterator, construct an IEnumVariant, and iterate over each
// IDispatch object in the enumeration.
for (auto match: dispatch.iter(L"Execute", L"A(b) c35 d_[x] yyy")) {
    // Get index from IDispatch match object.
    match.get(L"FirstIndex", index);
    printf("Index is %d\n", index);
}
```

## Types

AutoCOM uses strong-typing when possible to determine argument type. Argument type deduction can be grouped into 3 general cases:

- Literals
- Strong-Typing
- Type Wrappers

### Literals

AutoCOM supports C++ character (VT_I1), arithmetic, and string (VT_BSTR) literals.

For arithmetic literals, autocom has user-defined literals (in the `autocom::literals` namespace) to prevent implicit type-conversion or ambiguous overloads. The user-define literals are:

| Type         | Suffix |
|:------------:|:------:|
| CHAR         | _I1    |
| UCHAR        | _II1   |
| SHORT        | _I2    |
| USHORT       | _II2   |
| INT          | _INT   |
| UINT         | _IINT  |
| LONG         | _I4    |
| ULONG        | _II4   |
| LONGLONG     | _I8    |
| ULONGLONG    | _II8   |
| VARIANT_BOOL | _BOOL  |
| DATE         | _DATE  |
| SCODE        | _ERROR |

```cpp
using namespace autocom::literals;

dispatch.put("Char", '\0'_I1);
dispatch.put("UChar", '\0'_UI1);
dispatch.put("Short", 1_I2);
dispatch.put("Bool", 0_BOOL);
dispatch.put("Scode", 1_ERROR);
```

### Strong-Typing

AutoCOM uses C++'s strong-typing to determine VARTYPE-type when possible. The following types (include pointers to these types) are correctly deduced:

- CHAR
- UCHAR
- SHORT
- USHORT
- INT
- LONG
- ULONG
- LONGLONG
- ULONGLONG
- FLOAT
- DOUBLE
- CURRENCY
- DECIMAL
- IUnknown
- IDispatch
- SAFEARRAY

### Type Wrappers

For some types, especially type aliases, strong-typing is insufficient for type deduction. For example, `VARIANT_BOOL` (`VT_BOOL`) and `SHORT` (`VT_I2`) are both aliases of `short`, preventing VARTYPE deduction. AutoCOM provides type-safe value and reference wrappers to avoid VARTYPE ambiguity.

```cpp
using namespace autocom::wrappers;

DATE date = 0.0;
DATE *pdate = &date;

// by-value
dispatch.get("GetDate", date);              // ambiguous
dispatch.get("GetDate", G_DATE(date));      // safe, "Get" wrapper, `VT_DATE`
dispatch.get("GetDate", W_DATE(date));      // safe, generic wrapper, `VT_DATE`

// by-reference
dispatch.get("GetDate", pdate);             // ambiguous
dispatch.get("GetDate", G_DATE(pdate));     // safe, `VT_DATE | VT_BYREF`

// sanity checks
//dispatch.get("GetDate", G_DATE(5.0));     // cannot "Get" from R-value, won't compile
//dispatch.get("GetDate", W_DATE(5.0));     // cannot "Get" from R-value, won't compile
```

Each VARTYPE has "Get" (`G_`) and "Put" (`P_`) wrappers, as well as a generic `W_` wrapper (detecting L- or R-values and constructs the desired `G_` or `P_` wrapper, respectively). The type-wrapper functions are:

| Type         | Suffix    | Get        | Put        | Generic    |
|:------------:|:---------:|:----------:|:----------:|:----------:|
| CHAR         | _I1       | G_I1       | P_I1       | W_I1       |
| UCHAR        | _II1      | G_UI1      | P_UI1      | W_UI1      |
| SHORT        | _I2       | G_I2       | P_I2       | W_I2       |
| USHORT       | _II2      | G_II2      | P_II2      | W_II2      |
| INT          | _INT      | G_INT      | P_INT      | W_INT      |
| UINT         | _IINT     | G_IINT     | P_IINT     | W_IINT     |
| LONG         | _I4       | G_I4       | P_I4       | W_I4       |
| ULONG        | _II4      | G_II4      | P_II4      | W_II4      |
| LONGLONG     | _I8       | G_I8       | P_I8       | W_I8       |
| ULONGLONG    | _II8      | G_II8      | P_II8      | W_II8      |
| VARIANT_BOOL | _BOOL     | G_BOOL     | P_BOOL     | W_BOOL     |
| DATE         | _DATE     | G_DATE     | P_DATE     | W_DATE     |
| SCODE        | _ERROR    | G_ERROR    | P_ERROR    | W_ERROR    |
| DECIMAL      | _DECIMAL  | G_DECIMAL  | P_DECIMAL  | W_DECIMAL  |
| CURRENCY     | _CY       | G_CY       | P_CY       | W_CY       |
| SAFEARRAY*   | _ARRAY    | G_ARRAY    | P_ARRAY    | W_ARRAY    |
| VARIANT*     | _VARIANT  | G_VARIANT  | P_VARIANT  | W_VARIANT  |
| IUnknown*    | _UNKNOWN  | G_UNKNOWN  | P_UNKNOWN  | W_UNKNOWN  |
| IDispatch*   | _DISPATCH | G_DISPATCH | P_DISPATCH | W_DISPATCH |

## Examples

Examples of the late-binding interface can be found in the [example/late](/example/late) directory.
