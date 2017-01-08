
AutoCOM
=======

AutoCOM is a C++11 interface for the Component Object Model (COM) supporting MinGW and MSVC, inspired by [ComTypes](https://github.com/enthought/comtypes).

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

AutoCOM is a modern COM interface library for C++11 and supports either a [compile-time](/doc/CompileTimeBinding.md)  (analogous to `#import` statements) or [run-time](/doc/RunTimeBinding.md) (IDispatch) interface. AutoCOM manages object reference counts and parameter lists construction, for simple, cross-compiler COM usage. 

## Examples

**Compile-Time**

```cpp
#include "MSScriptControl.hpp"
namespace com = autocom;

int main(int argc, char *argv[])
{
    AutoComScriptControl script;

    com::Bstr command(L"VBScript");
    auto hr = script->Language(command.data());
    hr = script->AllowUI(VARIANT_FALSE);
    hr = script->Timeout(-1);

    com::Bstr statement(L"var a = 'test'");
    hr = script->ExecuteStatement(statement.data());

    return 0;
}
```

**Run-Time**

```cpp
#include "autocom.hpp"
namespace com = autocom;

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

## RAII

AutoCOM believes that resource initialization is acquisition, and that object destruction should free allocated resources. 

- Smart-pointers wrap COM objects, removing calls to `AddRef` and `Release`.
- Thread-local counters manage COM library initialization/uninitialization in COM object constructors/destructors. 
- BSTR, VARIANT, and SAFEARRAY have the RAII counterparts Bstr, Variant, SafeArray.

## Standard Template Library

AutoCOM aims to bridge Windows types with the Standard Template Library, to facilitate use of COM APIs in modern C++.

The `Bstr` and `SafeArray` RAII classes have interfaces comparable to `std::wstring` and `std::vector`, and can be implicitly converted to `BSTR` and `SAFEARRAY`.

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

AutoCOM uses COM ownership semantics for `put` and `method` calls: objects passed by value take ownership of the object, while those passed by reference are still owned by the user. Any existing references to objects passed by value are invalidated.

For `get` calls, the inverse is assumed: you take ownership of objects acquired by value, and do not take ownership of objects passed by reference.

Finally, ownership can be transfer from a VARIANT to a SafeArray or Bstr, so the new object is responsible for resource cleanup.

```cpp
Dispatch disp(L"SomeInterface");
Variant variant;

{
    disp.method(L"GetSafearray", &variant);                 // variant owns SA
    SafeArray array(variant);                               // array owns SA
}       // SA is deallocated since array goes out of scope
```

## Unicode

AutoCOM supports Unicode through Windows wide-string APIs, and assumes char-based strings are UTF-8 encoded, while wchar_t-based strings are UTF-16 encoded. 

To convert to UTF-8, the functions `UTF32_TO_UTF8` and `UTF16_TO_UTF8` are provided.

## Building

Simply clone, configure with CMake, and build.

```bash
git clone https://github.com/Alexhuszagh/autocom.git
git submodule update --init  
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

AutoCOM has been tested and confirmed to work with the following compilers/ecosystems:

- MSYS2, GCC 6.2
- Visual Studio 14 2015
- MinGW 5.4, Linux (running on Wine32)
- MXE

## Contributors

- Alex Huszagh

## License

MIT, see [license](LICENSE.md).
