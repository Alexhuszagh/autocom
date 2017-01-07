
AutoCOM
=======

AutoCOM is a C++11 interface for the Component Object Model (COM) supporting MinGW and MSVC, inspired by [ComTypes](https://github.com/enthought/comtypes).

**Table of Contents**

- [Motivation](#motivation)
- [Types](#types)
- [RAII](#raii)
- [Template Library](#template-library)
- [Ownership](#ownership)
- [Unicode](#unicode)
- [Building](#building)
- [Issues](#issues)
- [Platforms](#platforms)
- [Contributors](#contributors)
- [License](#license)

## Motivation

AutoCOM is a modern COM interface library for C++11 and supports either a compile-time (analogous to `#import` statements) or run-time (IDispatch) interface. For both interfaces, the `CoInitializeEx` is called on a per-thread basis prior to COM object construction, and `CoUninitialize` is called after the last COM object is destroyed. 

**Compile-Time**

AutoCOM uses COM's TypeLib interface to generate native C++ headers during compilation, to replace MSVC-only `#import` directives with native C++ headers, compatible for use with COM's QueryInterface. In addition, smart pointer (prefixed with "AutoCom") wrap the interface to automate `CoInitializeEx` and `CoUninitialize` during object creation and destruction, respectively.

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

AutoCOM uses thread-local reference-counting, C++ variadic templates, and perfect-forwarding to simplify COM's dynamic dispatch model through `IDispatch::Invoke`. Variant types are deduced and forwarded to automatically generate dispatch parameters.

Dynamic-dispatchers never looked so friendly:

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

## Types

The following only applies to the late-binding interface, where the parameter types for COM methods are unknown at compilation.

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

For some types, especially type aliases, automated type-detection can be insufficient (for example, `VARIANT_BOOL` and `SHORT` are both aliases of `short`, and `DATE` is an alias of `DOUBLE`). AutoCOM provides type-safe value and reference wrappers to avoid VARTYPE ambiguity.

```cpp
DATE date;
dispatch.get("GetDate", date);                      // ambiguous
dispatch.get("GetDate", autocom::GetDate(date));    // safe
```

Each COM type has both "Get" and "Put" wrappers. "Put" wrappers accept both L- and R-values, and move/copy the value (or pointer) into DISPPARAMS. "Get" wrappers only accept L-values, assigning directly to the reference.

## RAII

AutoCOM believes that resource initialization is acquisition, and that object destruction should cleanup any allocated resources. 

For COM interfaces, each COM object is wrapped in a `shared_ptr`, calling `IUnknown::Release` when the reference count drops to 0. The COM library itself is initialized and deinitialized on a thread-by-thread basis using thread-local reference counting. The use of smart pointers and automatic initialize removes the need for `AddRef` or `Release`, and guarantee exception-safe resource cleanup.

For BSTRs, VARIANTs, and SAFEARRAYs, AutoCOM includes RAII classes (Bstr, Variant, and SafeArray, respectively). These classes can inherit resources, or be forwarded to COM dispatchers. 

To facilitate resource ownership, AutoCOM differentiates between move and copy semantics: copy semantics copy resource data into the RAII class, leaving the old data intact, while move semantics take ownership of the resource.

```cpp
BSTR getString(...) {....}

// copy semantics, copies BSTR and creates RAII copy
auto string = getString();
com::Bstr copied(string);

// move semantics, takes ownership of BSTR, no copying occurs
com::Bstr moved(getString());
```

## Template Library

AutoCOM aims to facilitate use of COM APIs with the Standard Template Library, or STL, which provides the backbone of modern C++ programming.

The `Bstr` and `SafeArray` RAII classes have interfaces comparable to `std::wstring` and `std::vector`, including explicit conversions between them. Iteration, auto-ranges, and indexing are all supported.

```cpp
com::Bstr bstr(L"This is a string");

// auto-ranges
for (const wchar_t c: bstr) {
    std::cout << c << std::endl;
}

// element access
std::cout << bstr.front();                  // L'T'
std::cout << bstr.back();                   // L'g'
std::cout << bstr[1];                       // L'h'
std::cout << bstr.at(2);                    // L'i'

// conversions
std::wstring wstring(bstr);
com::Bstr copy(wstring);
```

## Ownership

AutoCOM uses COM ownership semantics for `put` and `method` calls: objects passed by value take ownership of the object, while those passed by reference are still owned by the user. References to existing objects passed by value should be considered invalid.

For `get` calls, the inverse is assumed: you take ownership of objects acquired by value, and do not take ownership of objects passed by reference.

Finally, ownership can be transfer from a VARIANT to a SafeArray or Bstr, so the new object is responsible for resource cleanup.

```cpp
autocom::Dispatch disp(L"SomeInterface");
autocom::Variant variant;

{
    disp.method(L"GetSafearray", &variant);                 // variant owns SA
    autocom::SafeArray array(variant);                      // array owns SA
}       // SA is deallocated since array goes out of scope
```

## Unicode

AutoCOM supports Unicode through Windows wide-string APIs, and assumes narrow strings are UTF-8 encoded, while wide strings are UTF-16 encoded.

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

To avoid this undefined behavior, AutoCOM expects the following:

- Pointers to objects passed by value (BSTR, IUnknown, IDispatch, SAFEARRAY) for `put` and `method` calls **must** be initialized.
- Pointers to objects passed by value for `get` calls may be uninitialized.
- For the early-binding interface, HRESULT values **must** be explicitly stored to avoid compiler mis-optimization.

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
