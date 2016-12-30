//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCom
 *  \brief Parse ITypeInfo data attributes.
 */

#pragma once

#include <autocom.hpp>

#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>


namespace autocom
{
namespace detail
{
// FORWARD
// -------

struct Enum;
struct Record;
struct Module;
struct Interface;
struct Dispatch;
struct CoClass;
struct Alias;
struct Union;
struct External;

// TYPES
// -----

typedef std::string Type;
typedef std::string Name;
typedef std::string Value;

// DESCRIPTIONS
typedef std::vector<Enum> EnumList;
typedef std::vector<Record> RecordList;
typedef std::vector<Module> ModuleList;
typedef std::vector<Interface> InterfaceList;
typedef std::vector<Dispatch> DispatchList;
typedef std::vector<CoClass> CoClassList;
typedef std::vector<Alias> AliasList;
typedef std::vector<Union> UnionList;
typedef std::vector<External> ExternalList;

// OBJECTS
// -------


/** \brief Base class for C++ code element.
 */
struct CppCode
{
    virtual std::string forward() const;
    virtual std::string header() const;
    virtual std::string source() const;
};


/** \brief Description for an enumerated type value.
 */
struct EnumValue: CppCode
{
    std::string name;
    std::string value;

    EnumValue() = default;
    EnumValue(const EnumValue&) = default;
    EnumValue & operator=(const EnumValue&) = default;
    EnumValue(EnumValue&&) = default;
    EnumValue & operator=(EnumValue&&) = default;

    EnumValue(const TypeInfo &info,
        const WORD index);

    virtual std::string header() const;
};


/** \brief Description for a variable without value.
 */
struct Parameter: CppCode
{
    std::string type;
    std::string name;

    Parameter() = default;
    Parameter(const Parameter&) = default;
    Parameter & operator=(const Parameter&) = default;
    Parameter(Parameter&&) = default;
    Parameter & operator=(Parameter&&) = default;

    Parameter(const TypeInfo &info,
        const WORD index);

    virtual std::string header() const;
};


/** \brief Description for a variable with value.
 */
struct Variable: CppCode
{
    std::string type;
    std::string name;
    std::string value;

    Variable() = default;
    Variable(const Variable&) = default;
    Variable & operator=(const Variable&) = default;
    Variable(Variable&&) = default;
    Variable & operator=(Variable&&) = default;

    Variable(const TypeInfo &info,
        const WORD index);

    virtual std::string header() const;
    virtual std::string source() const;
};


/** \brief Description for a function definition.
 */
struct Function: CppCode
{
    std::string decorator;
    std::string returns;
    std::string name;
    std::string doc;
    MEMBERID id;
    std::vector<Parameter> args;

    Function() = default;
    Function(const Function&) = default;
    Function & operator=(const Function&) = default;
    Function(Function&&) = default;
    Function & operator=(Function&&) = default;

    Function(const TypeInfo &info,
        const WORD index);

    std::string definition() const;
    std::string body() const;
    virtual std::string header() const;
    virtual std::string source() const;
};


///** \brief TODO:
// */
//template <typename T>
//class List
//{
//    std::string header;
//};


/** \brief Description for an enum type.
 */
struct Enum: CppCode
{
    std::string name;
    std::vector<EnumValue> values;

    Enum() = default;
    Enum(const Enum&) = default;
    Enum & operator=(const Enum&) = default;
    Enum(Enum&&) = default;
    Enum & operator=(Enum&&) = default;

    Enum(const TypeInfo &info,
        const TypeAttr &attr);

    virtual std::string header() const;
};


/** \brief Description for a struct without any methods.
 */
struct Record: CppCode
{
    Documentation documentation;
    ULONG size;
    std::vector<Parameter> fields;

    Record() = default;
    Record(const Record&) = default;
    Record & operator=(const Record&) = default;
    Record(Record&&) = default;
    Record & operator=(Record&&) = default;

    Record(const TypeInfo &info,
        const TypeAttr &attr);

    virtual std::string forward() const;
    virtual std::string header() const;
};


/** \brief Description for a module with only static functions and data.
 */
struct Module: CppCode
{
    std::vector<Function> functions;
    std::vector<Variable> constants;

    Module() = default;
    Module(const Module&) = default;
    Module & operator=(const Module&) = default;
    Module(Module&&) = default;
    Module & operator=(Module&&) = default;

    Module(const TypeInfo &info,
        const TypeAttr &attr);

    virtual std::string header() const;
    virtual std::string source() const;
};


/** \brief Description for a type with pure and virtual functions.
 */
struct Interface: CppCode
{
    Documentation documentation;
    Guid iid;
    WORD flags;
    std::string base;
    std::vector<Function> functions;

    Interface() = default;
    Interface(const Interface&) = default;
    Interface & operator=(const Interface&) = default;
    Interface(Interface&&) = default;
    Interface & operator=(Interface&&) = default;

    Interface(const TypeInfo &info,
        const TypeAttr &attr);

    virtual std::string header() const;
    virtual std::string source() const;
};


/** \brief Description for a COM object implementing IDispatch methods.
 */
struct Dispatch: Interface
{
    Dispatch() = default;
    Dispatch(const Dispatch&) = default;
    Dispatch & operator=(const Dispatch&) = default;
    Dispatch(Dispatch&&) = default;
    Dispatch & operator=(Dispatch&&) = default;

    Dispatch(const TypeInfo &info,
        const TypeAttr &attr);

    virtual std::string header() const;
};


/** \brief Description for a COM object implementing CoClass methods.
 */
struct CoClass: Dispatch
{
    CoClass() = default;
    CoClass(const CoClass&) = default;
    CoClass & operator=(const CoClass&) = default;
    CoClass(CoClass&&) = default;
    CoClass & operator=(CoClass&&) = default;

    CoClass(const TypeInfo &info,
        const TypeAttr &attr);

    virtual std::string header() const;
};


/** \brief Description for an alias for another type.
 */
struct Alias: CppCode
{
    Alias() = default;
    Alias(const Alias&) = default;
    Alias & operator=(const Alias&) = default;
    Alias(Alias&&) = default;
    Alias & operator=(Alias&&) = default;

    Alias(const TypeInfo &info,
        const TypeAttr &attr);

    virtual std::string header() const;
};


/** \brief Description for a union.
 */
struct Union: CppCode
{
    Union() = default;
    Union(const Union&) = default;
    Union & operator=(const Union&) = default;
    Union(Union&&) = default;
    Union & operator=(Union&&) = default;

    Union(const TypeInfo &info,
        const TypeAttr &attr);

    virtual std::string header() const;
};


/** \brief Description for a resource from another typelib.
 */
struct External: CppCode
{
    virtual std::string header() const;
};


/** \brief Generic description collection for a Typelib.
 */
struct Description
{
    EnumList enums;
    RecordList records;
    ModuleList modules;
    InterfaceList interfaces;
    DispatchList dispatchers;
    CoClassList coclasses;
    AliasList aliases;
    UnionList unions;
    ExternalList externals;
};

}   /* detail */


// PARSER
// ------


/** \brief TypeLib description.
 */
struct TypeLibDescription
{
    Guid guid;
    WORD major;
    WORD minor;
    Documentation documentation;
    detail::Description description;

    // TODO: need maps for records, etc...
    // TODO: methods...
    // TODO: NEED TO know....

    void parse(const TypeLib &tlib);
    //void parse
};


}   /* autocom */
