
Early-Binding AutoCOM
=====================

AutoCOM's early-binding interface produces native C++ headers analogous to MSVC's `#import` statements (compatible with legacy code) and interface wrappers for automatic COM object management.

**Table of Contents**

- [Design](#design)
  - [Definition Exportation](#definition-exportation)
  - [Wrappers](#wrappers)
- [Use](#use)
 - [Header Generation](#header-generation)
 - [CMake](#cmake)
- [Header Layout](#header-layout)
  - [CLSID](#clsid)
  - [Primitives & Enums](#primitives--enums)
  - [Forward Declarations & Aliases](#forward-declarations--aliases)
  - [Unions & Classes](#unions--classes)
  - [Method Signatures](#method-signatures)
- [Examples](#examples)

## Design

The interface consists of two parts: the export of type, constant, and function definitions from the COM DLL to allow direct function calls, and the addition of class wrappers to manage COM resources.

### Definition Exportation

AutoCOM parses the `ITypeLib` interface to generate C++ headers, for compilation-time method resolution. The following data is exported:

- Enums
- Structs (Records)
- Unions
- Typedefs (Aliases)
- Interfaces (IUnknown, IDispatch)
- CoClasses
- CLSIDs and IIDs (using the `CLSID_ClassName` and `IID_InterfaceName` convention)

### Wrappers

Smart pointer wrappers for the generated CoClasses are added to the headers, facilitate use of COM libraries. The wrappers methods `open` and `close`  manage COM initialization/uninitialization and create the COM interface, and are called during object construction/destruction.

For example, to use the `ScriptControl` interface from `MSScriptControl`, AutoCOM exports the `AutoComScriptControl` wrapper, which can be used like any COM pointer:

```cpp
{
    // The constructor calls `CoInitializeEx` before object creation,
    // then calls CoCreateInstance with the default CLSID and IID.
    AutoComScriptControl script;

    // The wrapper can be used like any COM object pointer.
    script->AllowUI(VARIANT_FALSE);

    // COM wrappers are copyable and movable.
    AutoComScriptControl copy(script);

// When a COM object goes out of scope, or you call `close`, 
// the object is freed with `IUnknown::Release`.
// If the destroyed object is the last remaining COM
// instance (per-thread), the destructor calls `CoUninitialize()`.
}
```

By default, the ComObject wrapper definition is analogous to:

```cpp
template <typename CoClass, typename Interface = CoClass>
class ComObject: public shared_ptr<Interface>
{...};
```

To use newer interfaces, specialize the ComObject with your desired interface:

```cpp
typedef ComObject<CoClass, Interface2> MyInterface;
MyInterface obj;
obj->NewMethod();
```

## Use

To use the early-binding interface, you must:

1. Generate COM headers
2. Include generated headers
3. Link with AutoCOM

This entire process can be automated with [CMake](#cmake) macros.

### Header Generation

AutoCOM includes an executable file, "autocom.exe", which generates C++ headers from a COM DLL. Both human-readable and CLSID-based headers are generated.

To generate headers, simply run the executable with the ProgID or CLSID desired, with an optional namespace:

```bash
$ # Generate input headers in namespace "wsh".
$ # Generates "IWshRuntimeLibrary.hpp" and 
$ # "F935DC20-1CF0-11D0-ADB9-00C04FD58A0B.hpp"
$ ./autocom.exe -progid="WScript.Shell.1" -ns=wsh
```

### CMake

Header generation and inclusion can be automated with the macro [AutoCOMConfigure](/cmake/autocom_configure.cmake) when using the CMake build system.

**Source**

First, add an include, starting with "@AUTOCOM", signifying that AutoCOM should process the include. The include has one mandatory key ("progid") and one optional property ("namespace"), delimited by ":".

For example, to generate a header in the namespace "wsh" from "WScript.Shell.1", we can add the following line to our source file.

```cpp
...
#include "@AUTOCOM:progid=WScript.Shell.1:namespace=wsh@"
...
```

**CMake Processing**

1. Add [autocom_configure](/cmake/autocom_configure.cmake) to the module path. 
2. Find the [AutoCOM installation](/cmake/FindAutoCOM.cmake) with `find_package(AutoCOM)`
3. Include the configuration macros with `include(autocom_configure)`
4. Configure each source file with `AutoCOMConfigure(output /path/to/source.cpp)`.
5. Add the output file to the resulting library/executable (`add_executable(${output}`)
6. Link with AutoCOM (`target_link_libraries(AutoCOM_LIBRARIES)`).

**Examples**

Examples can be found in the [example/cmake](/example/cmake) directory.

## Header Layout

Broadly, C++ headers are constructed with the following layout:

- CLSID
- Primitive & Enum Definitions
- Forward Declarations & Aliases
- Union & Class Definitions
- Method Signatures

### CLSID

The COM DLL CLSID is exported at the beginning of the header:

```cpp
DEFINE_GUID(CLSID_IWshRuntimeLibrary, 0xF935DC20, 0x1CF0, 0x11D0, 0xAD, 0xB9, 0x00, 0xC0, 0x4F, 0xD5, 0x8A, 0x0B);
```

### Primitives & Enums

Since primitives and enums do not depend on other COM-local definitions, they are declared immediately after the CLSID.

```cpp
// ENUMS
// -----

enum __WIDL_wshom_generated_name_00000000
{
    WshRunning = 0,
    WshFinished = 1,
    WshFailed = 2,
};
```

### Forward Declarations & Aliases

Since AutoCOM does not strictly preserve the ITypeLib item order, all class- and union-based definitions are forward-declared, and followed by typedefs for each alias.

```cpp
// FORWARD
// -------

struct IFolder;
...
typedef __WIDL_wshom_generated_name_00000000 WshExecStatus;
```

### Unions & Classes

All the remaining types, including unions, structs, interfaces, and CoClasses are defined, strictly preserving variable and virtual member function order. Static IIDs and flags are added to each interface definition, and CLSIDs to each CoClass definition, for data encapsulation without changing the object layout.

```cpp

DEFINE_GUID(IID_IWshShell, 0xF935DC21, 0x1CF0, 0x11D0, 0xAD, 0xB9, 0x00, 0xC0, 0x4F, 0xD5, 0x8A, 0x0B);

struct IWshShell: IDispatch
{
    static constexpr IID const &iid = IID_IWshShell;
    static constexpr WORD flags = 4432;
    virtual HRESULT __stdcall SpecialFolders(IWshCollection** arg0);
    ...
};
...

DEFINE_GUID(CLSID_IWshShell_Class, 0xF935DC22, 0x1CF0, 0x11D0, 0xAD, 0xB9, 0x00, 0xC0, 0x4F, 0xD5, 0x8A, 0x0B);

struct IWshShell_Class: IWshShell3
{
    static constexpr CLSID const &clsid = CLSID_IWshShell_Class;
    static constexpr IID const &iid = IID_IWshShell3;
};
```

### Method Signatures

Finally, although COM interfaces ideally are consistent for a given CLSID (all interfaces have identical virtual function signatures), methods can subtly differ with a given PROGID. For example, `IWshShell::Run` can take either a `ULONG*` or `INT*` status code, requiring argument type definitions from the function signature during compilation.

AutoCOM therefore exports the function argument count, return type, and argument types for each interface method at the end of the header. Using these type definitions can help resolve minor binary incompatibilities during compilation (see [wsh.cpp](/example/early/wsh.cpp) for an example).

```cpp
namespace signatures
{
// SIGNATURES
// ----------
namespace IFolder_NS
{
constexpr size_t Path_0_ArgCount = 1;
typedef HRESULT Path_0_Returns;
typedef BSTR* Path_0_Arg0;
...
}   /* IFolder_NS */
...
}   /* signatures */
```

## Examples

Examples of the early-binding interface can be found in the [example/early](/example/early) directory.
