//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup AutoCOM
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
struct Description;

// TYPES
// -----

typedef std::string Type;
typedef std::string Name;
typedef std::string Array;
typedef std::string Value;
typedef std::pair<Type, Array> DataType;

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

// MEMOIZERS
typedef std::unordered_set<Type> IgnoredMethods;
typedef std::unordered_map<Name, Type> Memo;
typedef std::unordered_map<Type, Type> InterfaceMap;
typedef std::unordered_map<Type, Guid> GUIDMap;

// OBJECTS
// -------


/** \brief Base class for C++ code element.
 */
struct CppCode
{
    virtual std::string forward() const;
    virtual std::string header() const;
};


/** \brief Description for an enumerated type value.
 */
struct EnumValue: CppCode
{
    Name name;
    Value value;

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
    Type type;
    Name name;

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
    Type type;
    Name name;
    Value value;

    Variable() = default;
    Variable(const Variable&) = default;
    Variable & operator=(const Variable&) = default;
    Variable(Variable&&) = default;
    Variable & operator=(Variable&&) = default;

    Variable(const TypeInfo &info,
        const WORD index);

    virtual std::string header() const;
};



/** \brief COM Dispatch property.
 */
struct Property: CppCode
{
    Property() = default;
    Property(const Property&) = default;
    Property & operator=(const Property&) = default;
    Property(Property&&) = default;
    Property & operator=(Property&&) = default;

    Property(const TypeInfo &info,
        const WORD index);

    virtual std::string header() const;
};


/** \brief Description for a function definition.
 */
struct Function: CppCode
{
    std::string decorator;
    std::string returns;
    Name name;
    std::string doc;
    MEMBERID id;
    SHORT offset;
    std::vector<Parameter> args;

    Function() = default;
    Function(const Function&) = default;
    Function & operator=(const Function&) = default;
    Function(Function&&) = default;
    Function & operator=(Function&&) = default;

    Function(const TypeInfo &info,
        const WORD index);

    std::string definition() const;
    virtual std::string header() const;
};


/** \brief Description for an enum type.
 */
struct Enum: CppCode
{
    Name name;
    std::vector<EnumValue> values;

    Enum() = default;
    Enum(const Enum&) = default;
    Enum & operator=(const Enum&) = default;
    Enum(Enum&&) = default;
    Enum & operator=(Enum&&) = default;

    Enum(const TypeInfo &info,
        Description & /*description*/);

    virtual std::string header() const;
};


/** \brief Description for a struct without any methods.
 */
struct Record: CppCode
{
    Name name;
    ULONG size;
    std::vector<Parameter> fields;

    Record() = default;
    Record(const Record&) = default;
    Record & operator=(const Record&) = default;
    Record(Record&&) = default;
    Record & operator=(Record&&) = default;

    Record(const TypeInfo &info,
        Description & /*description*/);

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
        Description & /*description*/);

    virtual std::string header() const;
};


/** \brief Description for a type with pure and virtual functions.
 *
 *  \param name                 Class name
 *  \param iid                  Class interface ID.
 *  \param flags                IDL flags for class.
 *  \param base                 Class interface inherits from
 *  \param object               Best Windows interface found
 */
struct Interface: CppCode
{
    Name name;
    Guid iid;
    WORD flags;
    Type base;
    Type object;
    std::vector<Property> properties;
    std::vector<Function> functions;

    Interface() = default;
    Interface(const Interface&) = default;
    Interface & operator=(const Interface&) = default;
    Interface(Interface&&) = default;
    Interface & operator=(Interface&&) = default;

    Interface(const TypeInfo &info,
        Description &description);

    IgnoredMethods & ignored() const;
    virtual std::string forward() const;
    virtual std::string header() const;
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
        Description &description);
};


/** \brief Description for a COM object implementing CoClass methods.
 *
 *  `added` provides a memo for the base interfaces, while `interfaces`
 *   keeps the sorted order of the interfaces. Some COM interfaces,
 *  like MS
 */
struct CoClass: CppCode
{
    Name name;
    Guid clsid;
    WORD flags;
    std::vector<Type> interfaces;
    std::unordered_set<Type> added;

    CoClass() = default;
    CoClass(const CoClass&) = default;
    CoClass & operator=(const CoClass&) = default;
    CoClass(CoClass&&) = default;
    CoClass & operator=(CoClass&&) = default;

    CoClass(const TypeInfo &info,
        Description &description);

    virtual std::string forward() const;
    virtual std::string header() const;
};


/** \brief Description for an alias for another type.
 */
struct Alias: CppCode
{
    Name name;
    Type type;

    Alias() = default;
    Alias(const Alias&) = default;
    Alias & operator=(const Alias&) = default;
    Alias(Alias&&) = default;
    Alias & operator=(Alias&&) = default;

    Alias(const TypeInfo &info,
        Description & /*description*/);

    virtual std::string header() const;
};


/** \brief Description for a union.
 */
struct Union: CppCode
{
    Name name;
    std::vector<Parameter> fields;

    Union() = default;
    Union(const Union&) = default;
    Union & operator=(const Union&) = default;
    Union(Union&&) = default;
    Union & operator=(Union&&) = default;

    Union(const TypeInfo &info,
        Description & /*description*/);

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
    // fields
    EnumList enums;
    RecordList records;
    ModuleList modules;
    InterfaceList interfaces;
    DispatchList dispatchers;
    CoClassList coclasses;
    AliasList aliases;
    UnionList unions;
    ExternalList externals;

    // redundancy checks
    InterfaceMap bases;
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

    void parse(const TypeLib &tlib);
};


}   /* autocom */
