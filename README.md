
AutoCOM
=======

AutoCOM is a C++11 interface for the Component Object Model (COM) supporting MinGW and MSVC, inspired by [ComTypes](https://github.com/enthought/comtypes).

**Table of Contents**

- [Motivation](#motivation)
- [Interface](#interface)
- [Types](#types)
- [RAII & Template Library](#raii-&-template-library)
- [Building](#building)
- [Issues](#issues)
- [Platforms](#platforms)
- [Contributors](#contributors)
- [License](#license)

## Motivation

AutoCOM is a modern COM interface library for C++11. AutoCOM supports both compile-time interface binding, or use of a dispatcher interface at run-time, with either MSVC or MinGW.

AutoCOM believes that resource initialization is acqusition. The COM interface is initialized on a per-thread basis during object construction, and uninitialized when the last object destructor is called. 

**Compile-Time**

AutoCOM uses COM's TypeLib interface to generate native C++ headers during compilation, to replace MSVC-only `#import` directives with native C++ headers, compatible for use with COM's QueryInterface.

**Run-Time**

AutoCOM uses thread-local reference-counting, C++ variadic templates, and perfect-forwarding to simplify COM's dynamic dispatch model through `IDispatch::Invoke`. Variant types are deduced and forwarded to automatically generate dispatch parameters.

Dynamic-dispatchers never looked so friendly:

```cpp
int main(int argc, char *argv[])
{
    com::Bstr text;
    com::Dispatch dispatch("VBScript.RegExp");
    dispatch.put("Pattern", L"\\w+");
    for (auto match: dispatch.iter("Execute", L"A(b) c35 d_[x] yyy")) {
        match.get("Value", text);
        printf("Match is %S\n", text);
    }
}
```

Compare this snippet to [code](https://gist.github.com/Alexhuszagh/c231052cb6e51868215608305fe4e797) invoking C++ IDispatch interface without AutoCOM.

## Interface

AutoCOM supports both early and late-binding interfaces, for both compile-time interface wrapping and dynamic dispatchers.

**Compile-Time**

```
#include "MSFileReader.hpp"
#include <cstdio>


int main(int argc, char *argv[])
{
    return 0;
}
```

**Run-Time**

```cpp
#include "autocom.hpp"
#include <cstdio>


int main(int argc, char *argv[])
{
    com::Dispatch dispatch("{1D23188D-53FE-4C25-B032-DC70ACDBDC02}");
    dispatch.method("Open", "Thermo.raw");

    LONG version;
    dispatch.method("GetVersionNumber", &version);
    printf("Version is %d\n", version);

    dispatch.method("Close");

    return 0;
}
```

## Types

The following only applies to the late-binding interface, where the parameter types for the COM method are unknown at compilation.

**Literals**

AutoCOM supports C++ literals, however, implicit type-conversion prevents automated type-deducation. For example, `dispatch.get("Put", 2)` could define any integer-literal. Type-safety is therefore only assured for string-literals (both narrow, UTF8 and wide, UTF16), which are internally converted to UTF16-encoded BSTRs.

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

For some types, which are aliases of one another, automated type-detection is insufficient (for example, `VARIANT_BOOL` and `SHORT` are both aliases of `short`, and `DATE` is an alias of `DOUBLE`). AutoCOM provides type-safe value and reference wrappers to avoid VARTYPE ambiguity.

```cpp
DATE date;
dispatch.get("GetDate", date);                      // ambiguous
dispatch.get("GetDate", auto com::GetDate(date));   // safe
```

Each COM type has both "Get" and "Put" wrappers. "Put" wrappers accept both L- and R-values, and move/copy the value (or pointer) into DISPPARAMS. "Get" wrappers only accept L-values, assigning directly to the reference.

## Template Library



## Building

Simply clone, configure with CMake, and build.

```bash
git clone https://github.com/Alexhuszagh/autocom.git
# git submodule update --init   # Uncomment to build tests
cd autocom/build
cmake ..                        # `-DBUILD_EXAMPLES=ON`, `-DBUILD_TESTS=ON`
make -j 5                       # "msbuild AutoCOM.sln" for MSVC
```

## Issues

Due to the automated memory management and variable forwarding/copying of arguments in AutoCOM, undefined behavior can appear to be highly variable.

For example, since AutoCOM calls `VariantClear` on all internal VARIANTs, all BSTRs are either reallocated or moved into the VARIANT, causing uninitialized BSTRs to invoke undefined behavior.

```cpp
// uninitialized
autocom:DispParams dp;
BSTR ptr;
dp.setArgs(BSTR());         // works
dp.setArgs(ptr);            // fails, SysAllocStringLen(ptr, SysStringLen(ptr))

// null initialized
ptr = nullptr;
dp.setArgs(ptr);            // works

// safe wrappers
Bstr wrapper;
dp.setArgs(Bstr());         // works
dp.setArgs(wrapper);        // works, wrappers initialize all pointers
```

To avoid this undesirable behavior, AutoCOM expects the following:

- Pointers to strings must be null-initialized, contain length data (BSTRs), or be null-terminated when passed as a value to a method call.
- Pointers may be uninitialized when used only for assignment, such as with `get` methods.

Any other issues can be reported on the [bug tracker](https://github.com/Alexhuszagh/autocom/issues).

## Platforms

AutoCOM has been tested and confirmed to work with the following compilers/ecosystems:

- MSYS2, GCC 6.2
- Visual Studio 14 2015
- MinGW 5.4, Linux (running on Wine32)
- MXE

## Contributors

- Alex Huszagh

## License

MIT, see [license](LICENSE.md).
