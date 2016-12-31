//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief C++ handle around ITypeInfo.
 */

#pragma once

#include "guid.hpp"

#include <oaidl.h>

#include <memory>


namespace autocom
{
// FORWARD
// -------

class TypeInfo;
class TypeAttr;
class TypeLib;
class TypeLibAttr;
struct Documentation;
struct DllEntry;
class VarDesc;
class FuncDesc;
class TypeDesc;
class ArrayDesc;
class IdlDesc;
class ElemDesc;
class ParamDesc;
class SafeArrayBound;

// TYPES
// -----

typedef std::shared_ptr<ITypeInfo> ITypeInfoPtr;
typedef std::shared_ptr<ITypeLib> ITypeLibPtr;
typedef std::shared_ptr<TYPEATTR> TYPEATTRPtr;
typedef std::shared_ptr<TLIBATTR> TLIBATTRPtr;
typedef std::shared_ptr<VARDESC> VARDESCPtr;
typedef std::shared_ptr<FUNCDESC> FUNCDESCPtr;

// FUNCTIONS
// ---------

/** \brief Create new handle to ITypeInfo object from IDispatch.
 */
ITypeInfo * newTypeInfo(IDispatch *dispatch);

/** \brief Create new handle to ITypeLib object from ITypeInfo.
 */
ITypeLib * newTypeLib(ITypeInfo *info);

/** \brief Create new handle to TYPEATTR object from ITypeInfo.
 */
TYPEATTR * newTypeAttr(ITypeInfo *info);

/** \brief Create new handle to VARDESC object from ITypeinfo.
 */
VARDESC * newVarDesc(ITypeInfo *info,
    const UINT index);

/** \brief Create new handle to TLIBATTR object from ITypeLib.
 */
TLIBATTR * newTypeLibAttr(ITypeLib *tlib);

// OBJECTS
// -------


/** \brief COM object wrapper for the ITypeInfo model.
 */
class TypeInfo
{
protected:
    ITypeInfoPtr ppv;

    friend bool operator==(const TypeInfo &left,
        const TypeInfo &right);
    friend bool operator!=(const TypeInfo &left,
        const TypeInfo &right);

public:
    TypeInfo() = default;
    TypeInfo(const TypeInfo&) = default;
    TypeInfo & operator=(const TypeInfo&) = default;
    TypeInfo(TypeInfo&&) = default;
    TypeInfo & operator=(TypeInfo&&) = default;

    TypeInfo(ITypeInfo *info);
    void open(ITypeInfo *info);

    // DATA
    TypeLib typelib() const;
    TypeAttr attr() const;
    Documentation documentation(const MEMBERID id) const;
    VarDesc vardesc(const UINT index) const;
    FuncDesc funcdesc(const UINT index) const;
    TypeInfo info(const HREFTYPE type) const;
    HREFTYPE reference(const UINT index) const;
    INT flags(const UINT index) const;
    DllEntry entry(const MEMBERID id,
        const INVOKEKIND invocation) const;
};


/** \brief COM object wrapper for the ITypeLib model.
 */
class TypeLib
{
protected:
    ITypeLibPtr ppv;

    friend bool operator==(const TypeLib &left,
        const TypeLib &right);
    friend bool operator!=(const TypeLib &left,
        const TypeLib &right);

public:
    TypeLib() = default;
    TypeLib(const TypeLib&) = default;
    TypeLib & operator=(const TypeLib&) = default;
    TypeLib(TypeLib&&) = default;
    TypeLib & operator=(TypeLib&&) = default;

    TypeLib(ITypeLib *tlib);
    void open(ITypeLib *tlib);

    // DATA
    TypeLibAttr attr() const;
    Documentation documentation(const INT index) const;
    UINT count() const;
    TypeInfo info(const UINT index) const;
};


/** \brief C++ Wrapper for a TYPEATTR structure.
 */
class TypeAttr
{
protected:
    ITypeInfoPtr ppv;
    TYPEATTRPtr attr;

public:
    TypeAttr() = default;
    TypeAttr(const TypeAttr&) = default;
    TypeAttr & operator=(const TypeAttr&) = default;
    TypeAttr(TypeAttr&&) = default;
    TypeAttr & operator=(TypeAttr&&) = default;

    TypeAttr(const ITypeInfoPtr &info);
    void open(const ITypeInfoPtr &info);

    // DATA
    explicit operator bool() const;
    Guid guid() const;
    LCID lcid() const;
    MEMBERID constructor() const;
    MEMBERID destructor() const;
    ULONG size() const;
    TYPEKIND kind() const;
    WORD functions() const;
    WORD variables() const;
    WORD interfaces() const;
    WORD vtblSize() const;
    WORD alignment() const;
    WORD flags() const;
    WORD major() const;
    WORD minor() const;
    TypeDesc alias() const;
    IdlDesc idl() const;
};


/** \brief C++ Wrapper for a TLIBATTR structure.
 */
class TypeLibAttr
{
protected:
    ITypeLibPtr ppv;
    TLIBATTRPtr attr;

public:
    TypeLibAttr() = default;
    TypeLibAttr(const TypeLibAttr&) = default;
    TypeLibAttr & operator=(const TypeLibAttr&) = default;
    TypeLibAttr(TypeLibAttr&&) = default;
    TypeLibAttr & operator=(TypeLibAttr&&) = default;

    TypeLibAttr(const ITypeLibPtr &tlib);
    void open(const ITypeLibPtr &tlib);

    // DATA
    explicit operator bool() const;
    Guid guid() const;
    LCID lcid() const;
    SYSKIND syskind() const;
    WORD major() const;
    WORD minor() const;
    WORD flags() const;
};


/** \brief Documentation for a type or library.
 *
 *  \param name         Name of item.
 *  \param doc          Documentation string for item.
 *  \param help         Identifier for help description.
 *  \param file         Fully-qualified path to file.
 */
struct Documentation
{
    std::string name;
    std::string doc;
    DWORD help = 0;
    std::string file;

    Documentation() = default;
    Documentation(const Documentation&) = default;
    Documentation & operator=(const Documentation&) = default;
    Documentation(Documentation&&) = default;
    Documentation & operator=(Documentation&&) = default;
};


/** \brief Documentation for DLL entry point.
 *
 *  \param dll          Name of DLL.
 *  \param function     Name of function.
 *  \param ordinal      Numeric identifier for function.
 */
struct DllEntry
{
    std::string dll;
    std::string function;
    WORD ordinal = 0;

    DllEntry() = default;
    DllEntry(const DllEntry&) = default;
    DllEntry & operator=(const DllEntry&) = default;
    DllEntry(DllEntry&&) = default;
    DllEntry & operator=(DllEntry&&) = default;
};


/** \brief C++ wrapper for a VARDESC structure.
 */
class VarDesc
{
protected:
    ITypeInfoPtr ppv;
    VARDESCPtr desc;

public:
    VarDesc() = default;
    VarDesc(const VarDesc&) = default;
    VarDesc & operator=(const VarDesc&) = default;
    VarDesc(VarDesc&&) = default;
    VarDesc & operator=(VarDesc&&) = default;

    VarDesc(const ITypeInfoPtr &info,
        const UINT index);
    void open(const ITypeInfoPtr &info,
        const UINT index);

    // DATA
    explicit operator bool() const;
    MEMBERID id() const;
    ElemDesc element() const;
    const VARIANT & variant() const;
    WORD flags() const;
    VARKIND kind() const;
};


/** \brief C++ wrapper for a FUNCDESC structure.
 */
class FuncDesc
{
protected:
    ITypeInfoPtr ppv;
    FUNCDESCPtr desc;

public:
    FuncDesc() = default;
    FuncDesc(const FuncDesc&) = default;
    FuncDesc & operator=(const FuncDesc&) = default;
    FuncDesc(FuncDesc&&) = default;
    FuncDesc & operator=(FuncDesc&&) = default;

    FuncDesc(const ITypeInfoPtr &info,
        const UINT index);
    void open(const ITypeInfoPtr &info,
        const UINT index);

    // DATA
    explicit operator bool() const;
    MEMBERID id() const;
    FUNCKIND kind() const;
    INVOKEKIND invocation() const;
    CALLCONV decoration() const;
    ElemDesc arg(const SHORT index) const;
    SHORT args() const;
    SHORT optional() const;
    SHORT offset() const;
    ElemDesc returnType() const;
    SHORT returnCount() const;
    WORD flags() const;
};


/** \brief C++ wrapper around TYPEDESC.
 */
class TypeDesc
{
protected:
    TYPEDESC desc;

public:
    TypeDesc() = default;
    TypeDesc(const TypeDesc&) = default;
    TypeDesc & operator=(const TypeDesc&) = default;
    TypeDesc(TypeDesc&&) = default;
    TypeDesc & operator=(TypeDesc&&) = default;

    TypeDesc(const TYPEDESC &desc);
    TypeDesc(TYPEDESC &&desc);

    // DATA
    VARTYPE vt() const;
    TypeDesc pointer() const;
    ArrayDesc array() const;
    HREFTYPE reference() const;
};


/** \brief C++ wrapper around ARRAYDESC
 */
class ArrayDesc
{
protected:
    ARRAYDESC desc;

public:
    ArrayDesc() = default;
    ArrayDesc(const ArrayDesc&) = default;
    ArrayDesc & operator=(const ArrayDesc&) = default;
    ArrayDesc(ArrayDesc&&) = default;
    ArrayDesc & operator=(ArrayDesc&&) = default;

    ArrayDesc(const ARRAYDESC &desc);
    ArrayDesc(ARRAYDESC &&desc);

    // DATA
    TypeDesc type() const;
    USHORT count() const;
    SafeArrayBound bound(const USHORT index) const;
};


/** \brief C++ wrapper around IDLDESC.
 */
class IdlDesc
{
protected:
    IDLDESC desc;

public:
    IdlDesc() = default;
    IdlDesc(const IdlDesc&) = default;
    IdlDesc & operator=(const IdlDesc&) = default;
    IdlDesc(IdlDesc&&) = default;
    IdlDesc & operator=(IdlDesc&&) = default;

    IdlDesc(const IDLDESC &desc);
    IdlDesc(IDLDESC &&desc);
};


/** \brief C++ wrapper around ELEMDESC.
 */
class ElemDesc
{
protected:
    ELEMDESC desc;

public:
    ElemDesc() = default;
    ElemDesc(const ElemDesc&) = default;
    ElemDesc & operator=(const ElemDesc&) = default;
    ElemDesc(ElemDesc&&) = default;
    ElemDesc & operator=(ElemDesc&&) = default;

    ElemDesc(const ELEMDESC &desc);
    ElemDesc(ELEMDESC &&desc);

    // DATA
    TypeDesc type() const;
    IdlDesc idl() const;
    ParamDesc param() const;
};


/** \brief C++ wrapper around PARAMDESC.
 */
class ParamDesc
{
protected:
    PARAMDESC desc;

public:
    ParamDesc() = default;
    ParamDesc(const ParamDesc&) = default;
    ParamDesc & operator=(const ParamDesc&) = default;
    ParamDesc(ParamDesc&&) = default;
    ParamDesc & operator=(ParamDesc&&) = default;

    ParamDesc(const PARAMDESC &desc);
    ParamDesc(PARAMDESC &&desc);

    // DATA
    ULONG size() const;
    VARIANTARG value() const;
    USHORT flags() const;
};


/** \brief C++ wrapper around SAFEARRAYBOUND.
 */
class SafeArrayBound
{
protected:
    SAFEARRAYBOUND bound;

public:
    SafeArrayBound() = default;
    SafeArrayBound(const SafeArrayBound&) = default;
    SafeArrayBound & operator=(const SafeArrayBound&) = default;
    SafeArrayBound(SafeArrayBound&&) = default;
    SafeArrayBound & operator=(SafeArrayBound&&) = default;

    SafeArrayBound(const SAFEARRAYBOUND &bound);
    SafeArrayBound(SAFEARRAYBOUND &&bound);

    // DATA
    ULONG size() const;
    LONG lower() const;
    ULONG upper() const;
};


}   /* autocom */
