//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCom
 *  \brief Parse ITypeInfo data attributes.
 */

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

// ATTRIBUTES
typedef std::tuple<std::string, long> EnumValue;

// OBJECTS
// -------


/** \brief Description for an enum type.
 */
struct Enum
{
    Enum() = default;
    Enum(const Enum&) = default;
    Enum & operator=(const Enum&) = default;
    Enum(Enum&&) = default;
    Enum & operator=(Enum&&) = default;

    Enum(const TypeInfo &info,
        const TypeAttr &attr);

    std::string name;
    std::vector<EnumValue> values;
};


/** \brief Description for a struct without any methods.
 */
struct Record
{
    Record() = default;
    Record(const Record&) = default;
    Record & operator=(const Record&) = default;
    Record(Record&&) = default;
    Record & operator=(Record&&) = default;

    Record(const TypeInfo &info,
        const TypeAttr &attr);
};


/** \brief Description for a module with only static functions and data.
 */
struct Module
{
    Module() = default;
    Module(const Module&) = default;
    Module & operator=(const Module&) = default;
    Module(Module&&) = default;
    Module & operator=(Module&&) = default;

    Module(const TypeInfo &info,
        const TypeAttr &attr);
};


/** \brief Description for a type with pure and virtual functions.
 */
struct Interface
{
    Interface() = default;
    Interface(const Interface&) = default;
    Interface & operator=(const Interface&) = default;
    Interface(Interface&&) = default;
    Interface & operator=(Interface&&) = default;

    Interface(const TypeInfo &info,
        const TypeAttr &attr);
};


/** \brief Description for a COM object implementing IDispatch methods.
 */
struct Dispatch
{
    Dispatch() = default;
    Dispatch(const Dispatch&) = default;
    Dispatch & operator=(const Dispatch&) = default;
    Dispatch(Dispatch&&) = default;
    Dispatch & operator=(Dispatch&&) = default;

    Dispatch(const TypeInfo &info,
        const TypeAttr &attr);
};


/** \brief Description for a COM object implementing CoClass methods.
 */
struct CoClass
{
    CoClass() = default;
    CoClass(const CoClass&) = default;
    CoClass & operator=(const CoClass&) = default;
    CoClass(CoClass&&) = default;
    CoClass & operator=(CoClass&&) = default;

    CoClass(const TypeInfo &info,
        const TypeAttr &attr);
};


/** \brief Description for an alias for another type.
 */
struct Alias
{
    Alias() = default;
    Alias(const Alias&) = default;
    Alias & operator=(const Alias&) = default;
    Alias(Alias&&) = default;
    Alias & operator=(Alias&&) = default;

    Alias(const TypeInfo &info,
        const TypeAttr &attr);
};


/** \brief Description for a union.
 */
struct Union
{
    Union() = default;
    Union(const Union&) = default;
    Union & operator=(const Union&) = default;
    Union(Union&&) = default;
    Union & operator=(Union&&) = default;

    Union(const TypeInfo &info,
        const TypeAttr &attr);
};


/** \brief Description for a resource from another typelib.
 */
struct External
{
    void parse();
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
