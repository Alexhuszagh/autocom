
Compile-Time AutoCOM
====================

AutoCOM's compile-time interface produces native C++ headers analogous to MSVC's `#import` statements, compatible with COM's QueryInterface.

**Table of Contents**

- [Design](#desgin)
  - [Header Generation](#header-generation)
  - [RAII Wrappers](#raii-wrappers)
- [Layout](#layout)

## Design

The interface consistes of two parts: the generation of C++ headers for method and type definitions, and RAII wrappers for COM interfaces.

### Header Generation

AutoCOM parses the `ITypeLib` interface to generate native C++ headers, for compilation-time method resolution.

### RAII Wrappers

Smart pointer wrappers for the generated CoClasses are added to the headers, which facilitate COM library initialization and interface construction. The wrappers provide `open` and `close` methods, initializing/deinitializing the COM interface, which are called during object construction/destruction.

For example, to use the `ScriptControl` interface from `MSScriptControl`, AutoCOM exports the `AutoComScriptControl` wrapper, which can be used like any COM pointer:

```cpp
{
    AutoComScriptControl script;                // initialize default interface
    script->AllowUI(VARIANT_FALSE);             // use as any COM object
}                                               // free object
```

By default, the ComObject resource is defined as follows:

```cpp
template <typename CoClass, typename Interface = CoClass>
class ComObject: public shared_ptr<Interface>
{...};
```

To use a newer interfaces, specialize the ComObject with the CoClass and interface of interest:

```cpp
typedef ComObject<CoClass, Interface2> MyInterface;
MyInterface obj;
obj->NewMethod();
```

## Layout

Static IIDs and object flags are added to each interface definition, and CLSIDs to each CoClass definition, for metadata encapsulation without changing the object layout. 

For example, an interface will be defined as follows:

```cpp
DEFINE_GUID(IID_IWshShell, 0xF935DC21, 0x1CF0, 0x11D0, 0xAD, 0xB9, 0x00, 0xC0, 0x4F, 0xD5, 0x8A, 0x0B);

struct IWshShell: IDispatch
{
    static constexpr IID const &iid = IID_IWshShell;
    static constexpr WORD flags = 4432;
    virtual HRESULT __stdcall SpecialFolders(IWshCollection** arg0);
    ...
};
```

And a CoClass is defined as:

```cpp
DEFINE_GUID(CLSID_IWshShell_Class, 0xF935DC22, 0x1CF0, 0x11D0, 0xAD, 0xB9, 0x00, 0xC0, 0x4F, 0xD5, 0x8A, 0x0B);

struct IWshShell_Class: IWshShell3
{
    static constexpr CLSID const &clsid = CLSID_IWshShell_Class;
    static constexpr IID const &iid = IID_IWshShell3;
};
```

The use of static class variables simplifies code using the template-driven API, while maintaining backwards compatibility.






