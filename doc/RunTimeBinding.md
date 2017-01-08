
Run-Time AutoCOM
================

AutoCOM's run-time interface uses thread-local reference-counting, C++ variadic templates, and perfect-forwarding to simplify dynamic method dispatch via `IDispatch::Invoke`. 

**Table of Contents**

- [Design](#desgin)
- [Types](#types)

## Design


## Types

The following only applies to the late-binding interface, where the parameter types for COM methods are unknown at compilation.

**Literals**

AutoCOM supports C++ literals, however, implicit type-conversion prevents automated type-deducation. For example, `dispatch.get("Put", 2)` could define any integer-literal. Type-safety is therefore only assured for string-literals (both narrow, UTF8 and wide, UTF16), which are internally converted to UTF16-encoded BSTRs.

For arithmetic literals, AutoCOM provides literal overloads in the `autocom::literals` namespace.

| Type      | SufFix |
|:---------:|:------:]
| CHAR      | _I1    |
| UCHAR     | _II1   |
| SHORT     | _I2    |
| USHORT    | _II2   |
| INT       | _INT   |
| UINT      | _IINT  |
| LONG      | _I4    |
| ULONG     | _II4   |
| LONGLONG  | _I8    |
| ULONGLONG | _II8   |

**Strong-Typing**

AutoCOM uses C++'s strong-typing to determine VARIANT-type when possible. The following types (include pointers to these types) are automatically deduced:

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

**Wrappers**

For some types, especially type aliases, automated type-detection can be insufficient (for example, `VARIANT_BOOL` and `SHORT` are both aliases of `short`, and `DATE` is an alias of `DOUBLE`). AutoCOM provides type-safe value and reference wrappers to avoid VARTYPE ambiguity.

```cpp
DATE date;
dispatch.get("GetDate", date);                      // ambiguous
dispatch.get("GetDate", autocom::GetDate(date));    // safe
```

Each COM type has both "Get" and "Put" wrappers. "Put" wrappers accept both L- and R-values, and move/copy the value (or pointer) into DISPPARAMS. "Get" wrappers only accept L-values, assigning directly to the reference.
