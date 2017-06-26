AutoCOM
=======

AutoCOM is a C++11 interface for the Component Object Model (COM), supporting MinGW and MSVC, inspired by [ComTypes](https://github.com/enthought/comtypes).

[![Build Status](https://travis-ci.org/Alexhuszagh/autocom.svg?branch=master)](https://travis-ci.org/Alexhuszagh/AutoCOM)
[![Build Status](https://tea-ci.org/api/badges/Alexhuszagh/autocom/status.svg)](https://tea-ci.org/Alexhuszagh/autocom)
[![Build status](https://ci.appveyor.com/api/projects/status/jx4mmgo25myx9u9i?svg=true)](https://ci.appveyor.com/project/Alexhuszagh/autocom)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/12989/badge.svg)](https://scan.coverity.com/projects/alexhuszagh-autocom)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/01fcadd11fae44f7a21b5a5272623b5d)](https://www.codacy.com/app/Alexhuszagh/autocom?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Alexhuszagh/autocom&amp;utm_campaign=Badge_Grade)
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/Alexhuszagh/AutoCOM/blob/master/LICENSE.md)

**Table of Contents**

- [Description](#description)
- [Examples](#examples)
- [RAII](#raii)
- [Standard Template Library](#standard-template-library)
- [Ownership](#ownership)
- [Unicode](#unicode)
- [Building](#building)
- [Issues](#issues)
- [Platforms](#platforms)
- [Contributors](#contributors)
- [License](#license)

## Description

AutoCOM is a COM interface library, supporting both [early-binding](/doc/EarlyBinding.md) and [late-binding](/doc/LateBinding.md) interfaces. AutoCOM internally manages allocated resources and interface initialization, for simple, cross-compiler COM use.

## Examples

**Early-Binding**

```cpp
#include "MSScriptControl.hpp"
namespace com = autocom;

int main(int argc, char *argv[])
{
    AutoComScriptControl script;

    com::Bstr command(L"VBScript");
    auto hr = script->Language(command);
    hr = script->AllowUI(VARIANT_FALSE);
    hr = script->Timeout(-1);

    com::Bstr statement(L"var a = 'test'");
    hr = script->ExecuteStatement(statement);

    return 0;
}
```

**Late-Binding**

```cpp
#include <iostream>
#include <autocom.hpp>
namespace com = autocom;

int main(int argc, char *argv[])
{
    com::Bstr text;
    com::Dispatch dispatch(L"VBScript.RegExp");
    dispatch.put(L"Pattern", L"\\w+");
    for (auto match: dispatch.iter(L"Execute", L"A(b) c35 d_[x] yyy")) {
        match.get(L"Value", text);
        std::wcout << L"Match is '" <<  text << "'\n";
    }
}
```

Compare this snippet to [code](https://gist.github.com/Alexhuszagh/c231052cb6e51868215608305fe4e797) invoking C++ IDispatch interface without AutoCOM.

## RAII

AutoCOM believes that resource acquisition is initialization (RAII): that construction should initialize COM resources and destruction should free allocated resources. To ensure resource management is tied to object lifecycle, AutoCOM includes the following features:

- Smart-pointers wrappers for COM objects, calling `Release` when the last instance is destroyed.
- Automated `CoInitializeEx` and `CoUninitialize` calls via thread-local COM instance counters. 
- Wrappers for BSTR (Bstr), VARIANT (Variant), and SAFEARRAY (SafeArray) in the `autocom` namespace.

## Standard Template Library

AutoCOM adds Standard Template Library (STL) container methods to Windows-specific containers, to facilitate use of COM APIs in modern C++.

`Bstr` and `SafeArray` have interfaces comparable to `std::wstring` and `std::vector`, and are implicitly convertible to `BSTR` and `LPSAFEARRAY`, respectively.

```cpp
com::Bstr string(L"This is a string");
// auto-ranges
for (const wchar_t c: string) {
    std::cout << c << std::endl;
}

// element access
std::cout << string.front();                  // L'T'
std::cout << string.back();                   // L'g'
std::cout << string[1];                       // L'h'
std::cout << string.at(2);                    // L'i'

// conversions
BSTR bstr(string);
com::Bstr copy(bstr);
```

## Ownership

AutoCOM uses COM ownership semantics for `put` and `method` calls: the COM library takes ownership of "in parameters" passed by value, modifies "in-out parameters" passed by value, while the ownership of objects passed by reference remains unchanged. The caller must call `AddRef` on any COM objects passed as "in-out parameters". For "in-parameters", any existing references to objects passed by value are invalidated.

For `get` calls, the caller takes ownership of objects acquired by value, while the ownership of objects passed by reference remains unchanged.

Finally, SafeArrays and Bstrs can inherit resources from VARIANTs/Variants through object construction or assignment, so VARIANTs can be safely discarded after method calls.

```cpp
Dispatch disp(L"SomeInterface");
Variant variant;

{
    // Call method that constructs a `SAFEARRAY` and sets a pointer
    // in `variant.parray`. `variant` owns the resource, and will
    // deallocate it on object destruction.
    disp.method(L"GetSafearray", &variant);

    // Inherit resource from `Variant`, emptying the variant.
    // `array` now owns the `SAFEARRAY`, and will deallocate it
    // on destruction.
    SafeArray<int> array(variant);
    for (const int value: array) {
        // do something with data
    }

// `array` is destroyed, freeing the `SAFEARRAY`, while the existing
// `variant` remains empty.
}
```

## Unicode

AutoCOM supports Unicode through Windows wide-string APIs, and assumes `char`-based strings are UTF-8 encoded, while `wchar_t`-based strings are UTF-16 encoded. 

Text passed by value as an in-parameter, or for method lookup can be passed using string literals, STL strings, or BSTR/Bstr, however, for performance reasons, wide strings should be preferred when possible.

## Building

Simply clone, configure with CMake, and build.

```bash
git clone https://github.com/Alexhuszagh/autocom.git
git submodule update --init  --recursive
cd autocom/build
cmake ..                        # `-DBUILD_EXAMPLES=ON`, `-DBUILD_TESTS=ON`
make -j 5                       # "msbuild AutoCOM.sln" for MSVC
```

## Issues

To avoid this undefined behavior, AutoCOM expects the following:

- Pointers to objects passed by value (BSTR, IUnknown, IDispatch, SAFEARRAY) for `put` and `method` calls **must** be initialized.
- Pointers to objects passed by value for `get` calls may be uninitialized.

Any other issues can be reported on the [bug tracker](https://github.com/Alexhuszagh/autocom/issues).

## Platforms

AutoCOM is continually built with the following compiler and compiler versions:

- MinGW 5.3.0 (MXE, MinGW, and MSYS2) 
- Visual Studio 14 2015
- Visual Studio 15 2017

## Contributors

- Alex Huszagh

## License

MIT, see [license](LICENSE.md).
