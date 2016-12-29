//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCom
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
class VarDesc;

// TYPES
// -----

typedef std::shared_ptr<ITypeInfo> ITypeInfoPtr;
typedef std::shared_ptr<ITypeLib> ITypeLibPtr;
typedef std::shared_ptr<TYPEATTR> TYPEATTRPtr;
typedef std::shared_ptr<TLIBATTR> TLIBATTRPtr;
typedef std::shared_ptr<VARDESC> VARDESCPtr;

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
VARDESC * newVarDesc(ITypeInfo *info);

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
    VarDesc vardesc() const;
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
    TYPEDESC alias() const;
    IDLDESC idl() const;
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

    VarDesc(const ITypeInfoPtr &info);
    void open(const ITypeInfoPtr &info);

    // DATA
    explicit operator bool() const;
    // TODO...
};


}   /* autocom */
