//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief Parse ITypeInfo data attributes.
 */

#include "parser.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>             // TODO: REMOVE
#include <sstream>
#include <stdexcept>


namespace autocom
{
namespace detail
{
// CONSTANTS
// ---------

std::unordered_map<VARTYPE, std::string> TYPE_NAMES = {
    { VT_I1,        "CHAR"          },
    { VT_UI1,       "UCHAR"         },
    { VT_I2,        "SHORT"         },
    { VT_UI2,       "USHORT"        },
    { VT_I4,        "LONG"          },
    { VT_UI4,       "ULONG"         },
    { VT_I8,        "LONGLONG"      },
    { VT_UI8,       "ULONGLONG"     },
    { VT_INT,       "INT"           },
    { VT_UINT,      "UINT"          },
    { VT_R4,        "FLOAT"         },
    { VT_R8,        "DOUBLE"        },
    { VT_BOOL,      "VARIANT_BOOL"  },
    { VT_HRESULT,   "HRESULT"       },
    { VT_LPSTR,     "LPSTR"         },
    { VT_LPWSTR,    "LPWSTR"        },
    { VT_CY,        "CURRENCY"      },
    { VT_DATE,      "DATE"          },
    { VT_BSTR,      "BSTR"          },
    { VT_DECIMAL,   "DECIMAL"       },
    { VT_ERROR,     "SCODE"         },
    { VT_DISPATCH,  "IDispatch *"   },
    { VT_UNKNOWN,   "IUnknown *"    },
    { VT_VARIANT,   "VARIANT"       },
    { VT_VOID,      "void"          },
};

std::unordered_map<CALLCONV, std::string, EnumHash> DECORATIONS = {
    { CC_FASTCALL,   "__fastcall" },
    { CC_CDECL,      "__cdecl"    },
    { CC_MSCPASCAL,  ""           },
    { CC_PASCAL,     "__pascal"   },
    { CC_MACPASCAL,  ""           },
    { CC_STDCALL,    "__stdcall"  },
    { CC_FPFASTCALL, ""           },
    { CC_SYSCALL,    "__syscall"  },
    { CC_MPWCDECL,   ""           },
    { CC_MPWPASCAL,  ""           },
};

/** \brief Ingored methods redundantly listed in typelib.
 *
 *  This could be overly-excluding functions, since IUnknown
 *  interfaces are free to any method besides the first three.
 */
std::unordered_map<Type, IgnoredMethods> IGNORED = {
    { "IUnknown", {
        "QueryInterface",
        "AddRef",
        "Release",
    }},
    { "IDispatch", {
        "QueryInterface",
        "AddRef",
        "Release",
        "GetTypeInfoCount",
        "GetTypeInfo",
        "GetIDsOfNames",
        "Invoke"
    }},
    { "IClassFactory", {
        "QueryInterface",
        "AddRef",
        "Release",
        "CreateInstance",
        "LockServer",
    }},
};


// SORTING
// -------


/** \brief Sort functions by vtable offset.
 */
bool functionKey(const Function &left,
    const Function &right)
{
    return left.offset < right.offset;
}


// FUNCTIONS
// ---------


/** \brief Parse description from a typelib.
 */
template <typename Description>
void parseDescription(Description &description,
    const TypeLib &tlib)
{
    auto attr = tlib.attr();
    description.guid = attr.guid();
    description.major = attr.major();
    description.minor = attr.minor();
    description.documentation = tlib.documentation(-1);
}


/** \brief Parse item from TypeInfo of subelement.
 */
template <typename Description>
void parseItem(Description &desc,
    const TypeInfo &info)
{
    switch (info.attr().kind()) {
        case TKIND_ENUM:
            desc.enums.emplace_back(Enum(info, desc));
            break;
        case TKIND_RECORD:
            desc.records.emplace_back(Record(info, desc));
            break;
        case TKIND_MODULE:
            desc.modules.emplace_back(Module(info, desc));
            break;
        case TKIND_INTERFACE:
            desc.interfaces.emplace_back(Interface(info, desc));
            break;
        case TKIND_DISPATCH:
            try {
                // dual interface
                auto href = info.reference(-1);
                desc.interfaces.emplace_back(Interface(info.info(href), desc));
            } catch (ComMethodError) {
                // no dual interface
                desc.dispatchers.emplace_back(Dispatch(info, desc));
            }
            break;
        case TKIND_COCLASS:
            desc.coclasses.emplace_back(CoClass(info, desc));
            break;
        case TKIND_ALIAS:
            desc.aliases.emplace_back(Alias(info, desc));
            break;
        case TKIND_UNION:
            desc.unions.emplace_back(Union(info, desc));
            break;
    }
}


/** \brief Get variable type name from VARTYPE descriptor.
 */
std::string getTypeName(const TypeInfo &info,
    const TypeDesc &desc)
{
    auto it = TYPE_NAMES.find(desc.vt());
    if (it != TYPE_NAMES.end()) {
        return it->second;
    } else if (desc.vt() == VT_CARRAY) {
        // get type description for C-style array: char[5][4]
        auto array = desc.array();
        auto name = getTypeName(info, array.type());
        for (USHORT index = 0; index < array.count(); ++index) {
            name += "[" + std::to_string(array.bound(index).size()) + "]";
        }
        return name;
    } else if (desc.vt() == VT_PTR) {
        // return pointer type
        return getTypeName(info, desc.pointer()) + "*";
    } else if (desc.vt() == VT_USERDEFINED) {
        return info.info(desc.reference()).documentation(-1).name;
    } else if (desc.vt() == VT_SAFEARRAY) {
        return "SAFEARRAY";
    }

    // VT_VOID
    throw std::invalid_argument("Invalid type: " + std::to_string(desc.vt()));
}


/** \brief Get value name from variant.
 */
std::string getValueName(const VARIANT &variant)
{
    switch (variant.vt) {
        case VT_I1:
            return std::string(1, variant.cVal);
        case VT_UI1:
            return std::string(1, variant.bVal);
        case VT_I2:
            return std::to_string(variant.iVal);
        case VT_UI2:
            return std::to_string(variant.uiVal);
        case VT_I4:
            return std::to_string(variant.lVal);
        case VT_UI4:
            return std::to_string(variant.ulVal);
        case VT_I8:
            return std::to_string(variant.llVal);
        case VT_UI8:
            return std::to_string(variant.ullVal);
        case VT_INT:
            return std::to_string(variant.intVal);
        case VT_UINT:
            return std::to_string(variant.uintVal);
        case VT_R4:
            return std::to_string(variant.fltVal);
        case VT_R8:
            return std::to_string(variant.dblVal);
        case VT_BOOL:
            return variant.boolVal ? "true" : "false";
        case VT_BSTR:
            return std::string(Bstr(variant.bstrVal));
        case VT_ERROR:
            return std::to_string(variant.scode);
        case VT_DATE:
            return std::to_string(variant.date);
        case VT_LPSTR:
        case VT_LPWSTR:
        case VT_HRESULT:
        case VT_DISPATCH:
        case VT_UNKNOWN:
        case VT_VARIANT:
        case VT_DECIMAL:
        case VT_CY:
        default:
            break;
    }

    throw std::invalid_argument("Unrecognized type: " + std::to_string(variant.vt));
}


// OBJECTS
// -------


/** \brief Get forward declaration definition.
 */
std::string CppCode::forward() const
{
    assert(false);
    return "";
}


/** \brief Get header definition.
 */
std::string CppCode::header() const
{
    assert(false);
    return "";
}


/** \brief Extract enum value data.
 */
EnumValue::EnumValue(const TypeInfo &info,
    const WORD index)
{
    // get descriptors
    auto vd = info.vardesc(index);
    auto variant = vd.variant();

    name = info.documentation(vd.id()).name;
    value = getValueName(variant);
}


/** \brief Get representation in header.
 */
std::string EnumValue::header() const
{
    return name + " = " + value;
}


/** \brief Parse variable without assigned value.
 */
Parameter::Parameter(const TypeInfo &info,
    const WORD index)
{
    // get descriptors
    auto vd = info.vardesc(index);
    assert(vd.kind() == VAR_PERINSTANCE);

    type = getTypeName(info, vd.element().type());
    name = info.documentation(vd.id()).name;
}


/** \brief Get representation in header.
 */
std::string Parameter::header() const
{
    return type + " " + name;
}


/** \brief Parse variable with assigned value.
 */
Variable::Variable(const TypeInfo &info,
    const WORD index)
{
    // get descriptors
    auto vd = info.vardesc(index);
    assert(vd.kind() == VAR_CONST);

    type = getTypeName(info, vd.element().type());
    name = info.documentation(vd.id()).name;
    value = getValueName(vd.variant());
}


/** \brief Get representation in header.
 */
std::string Variable::header() const
{
    return "extern " + type + " " + name;
}


/** \brief Parse dispatch property definition.
 */
Property::Property(const TypeInfo &info,
    const WORD index)
{}


/** \brief Get representation in header.
 */
std::string Property::header() const
{
    assert(false);
}


/** \brief Parse function definition.
 */
Function::Function(const TypeInfo &info,
    const WORD index)
{
    // get descriptors
    //std::cout << "auto fd = info.funcdesc(index);" << std::endl;
    auto fd = info.funcdesc(index);
    //std::cout << "auto documentation = info.documentation(fd.id());" << std::endl;
    auto documentation = info.documentation(fd.id());

    decorator = DECORATIONS[fd.decoration()];
    returns = getTypeName(info, fd.returnType().type());
    name = documentation.name;
    doc = documentation.doc;
    id = fd.id();
    offset = fd.offset();
    args.resize(fd.args());
    for (SHORT index = 0; index < fd.args(); ++index) {
        args[index].type = getTypeName(info, fd.arg(index).type());
        args[index].name = "arg" + std::to_string(index);
    }
}


/** \brief Get function definition line.
 */
std::string Function::definition() const
{
    std::ostringstream stream;
    stream << name << "(";
    if (!args.empty()) {
        for (size_t index = 0; index < args.size() - 1; ++index) {
            stream << args[index].header() << ", ";
        }
        stream << args.back().header();
    }
    stream << ")";

    return stream.str();
}


/** \brief Get representation in header.
 */
std::string Function::header() const
{
    std::ostringstream stream;
    stream << decorator << " " << returns << " " << definition() << ";";

    return stream.str();
}


/** \brief Initialize Enum method description from TypeInfo.
 */
Enum::Enum(const TypeInfo &info,
    Description & /*description*/)
{
    name = info.documentation(-1).name;
    for (WORD index = 0; index < info.attr().variables(); ++index) {
        values.emplace_back(EnumValue(info, index));
    }
}


/** \brief Write enumeration to string for header.
 */
std::string Enum::header() const
{
    std::ostringstream stream;
    stream << "enum " << name << "\r\n"
           << "{\r\n";
    for (const auto &value: values) {
        stream << "    " << value.header() << ",\r\n";
    }
    stream << "};\r\n";

    return stream.str();
}


/** \brief Initialize Record method description from TypeInfo.
 */
Record::Record(const TypeInfo &info,
    Description & /*description*/)
{
    auto attr = info.attr();
    name = info.documentation(-1).name;
    size = attr.size();
    for (WORD index = 0; index < attr.variables(); ++index) {
        fields.emplace_back(Parameter(info, index));
    }
}


/** \brief Forward declaration for struct.
 */
std::string Record::forward() const
{
    return "struct " + name + ";";
};


/** \brief Write struct to string for header.
 */
std::string Record::header() const
{
    std::ostringstream stream;
    stream << "struct " << name << "\r\n"
           << "{\r\n";
    for (const auto &field: fields) {
        stream << "    " << field.header() << ";\r\n";
    }
    stream << "};\r\n";
    stream << "static_assert(sizeof(" << name
           << ") == " << size
           << ", \"AutoCOM: Invalid struct size.\");\r\n";

    return stream.str();
}


/** \brief Initialize Module method description from TypeInfo.
 *
 *  \warning This code should be assumed to be buggy, since no COM
 *  DLLs installed on Wine use TKIND_MODULE.
 */
Module::Module(const TypeInfo &info,
    Description & /*description*/)
{
    auto attr = info.attr();

    // functions
    for (WORD index = 0; index < attr.functions(); ++index) {
        functions.emplace_back(Function(info, index));
    }
    std::sort(functions.begin(), functions.end(), functionKey);

    // variables
    for (WORD index = 0; index < attr.variables(); ++index) {
        constants.emplace_back(Variable(info, index));
    }
}


/** \brief Write module to string for header.
 */
std::string Module::header() const
{
    std::ostringstream stream;
    for (const auto &item: functions) {
        stream << item.header() << "\r\n";
    }
    for (const auto &item: constants) {
        stream << "const " << item.header() << ";\r\n";
    }

    return stream.str();
}


/** \brief Initialize Interface method description from TypeInfo.
 */
Interface::Interface(const TypeInfo &info,
    Description &description)
{
    auto attr = info.attr();
    auto &bases = description.bases;

    // parse interface attributes
    name = info.documentation(-1).name;
    iid = attr.guid();
    flags = attr.flags();

    // parse base classes
    if (attr.interfaces()) {
        base = info.info(info.reference(0)).documentation(-1).name;
        object = base;
        while (IGNORED.find(object) == IGNORED.end()) {
            std::cout << object << std::endl;
            object = bases.at(object);
        }
        bases[name] = object;
    }

    for (WORD index = 0; index < attr.functions(); ++index) {
        Function function(info, index);
        if (ignored().find(function.name) == ignored().end())  {
            functions.emplace_back(std::move(function));
        }
    }
    std::sort(functions.begin(), functions.end(), functionKey);
}


/** \brief Get methods ignored by interface.
 */
IgnoredMethods & Interface::ignored() const
{
    return IGNORED[object];
}


/** \brief Forward declaration for interface.
 */
std::string Interface::forward() const
{
    return "struct " + name + ";";
};


/** \brief Write interface to string for header.
 */
std::string Interface::header() const
{
    std::ostringstream stream;

    // TODO: I need to fix how these are compiled in....
    stream << "constexpr char IID_" << name << "[]"
           << " = \"" << iid.string() << "\";\r\n\r\n";
    // class definition and base class
    stream << "struct " << name;
    if (!base.empty()) {
        stream << ": " << base;
    }
    // initialize with static, constexpr values which do not add to struct
    stream << "\r\n"
          << "{\r\n"
           << "    static constexpr WORD flags = " << flags << ";\r\n";
    // properties
    for (const auto &item: properties) {
        stream << "    " << item.header() << "\r\n";
    }
    // functions
    for (const auto &item: functions) {
        stream << "    " << item.header() << "\r\n";
    }

    stream << "};\r\n";

    return stream.str();
}


/** \brief Initialize Dispatch method description from TypeInfo.
 */
Dispatch::Dispatch(const TypeInfo &info,
        Description &description):
    Interface(info, description)
{
    auto attr = info.attr();
    for (WORD index = 0; index < attr.variables(); ++index) {
        // TODO: these are dispatch properties, assert(false) and check
        std::cout << "Dispatch property: " << index << std::endl;
        assert(false);
        //properties.emplace_back(Property(info, index));
    }
}


/** \brief Initialize CoClass method description from TypeInfo.
 */
CoClass::CoClass(const TypeInfo &info,
    Description &description)
{
    // parse interface attributes
    auto attr = info.attr();
    name = info.documentation(-1).name;
    clsid = attr.guid();
    flags = attr.flags();

    // basic assumption
    assert(attr.functions() == 0);
    assert(attr.variables() == 0);

    for (WORD index = 0; index < attr.interfaces(); ++index) {
        auto tinfo = info.info(info.reference(0));
        interfaces.emplace_back(tinfo.documentation(-1).name);
    }
}


/** \brief Forward declaration for interface.
 */
std::string CoClass::forward() const
{
    return "struct " + name + ";";
};


/** \brief Write coclass to string for header.
 */
std::string CoClass::header() const
{
    assert(interfaces.size());

    std::ostringstream stream;
    stream << "struct " << name << ": ";
    for (auto it = interfaces.begin(); it < interfaces.end() - 1; ++it) {
        stream << *it << ", ";
    }
    stream << interfaces.back() << "\r\n";
    stream << "{};\r\n";

    return stream.str();
}


/** \brief Initialize Alias method description from TypeInfo.
 */
Alias::Alias(const TypeInfo &info,
    Description &description)
{
    name = info.documentation(-1).name;
    type = getTypeName(info, info.attr().alias());
}


/** \brief Write alias to string for header.
 */
std::string Alias::header() const
{
    return "typedef " + name + " " + type + ";";
}


/** \brief Initialize Union method description from TypeInfo.
 */
Union::Union(const TypeInfo &info,
    Description & /*description*/)
{
    name = info.documentation(-1).name;

    // parse interface attributes
    auto attr = info.attr();
    for (WORD index = 0; index < attr.variables(); ++index) {
        fields.emplace_back(Parameter(info, index));
    }
}


/** \brief Write union to string for header.
 */
std::string Union::header() const
{
    std::ostringstream stream;
    stream << "union " << name << "\r\n"
           << "{\r\n";
    for (const auto &field: fields) {
        stream << "    " << field.header() << ";\r\n";
    }
    stream << "};\r\n";

    return stream.str();
}


/** \brief Write external library to string for header.
 */
std::string External::header() const
{
    assert(false);
}


}   /* detail */


// PARSER
// ------


/** \brief Parse TypeLib from COM object.
 */
void TypeLibDescription::parse(const TypeLib &tlib)
{
    // get library definitions
    detail::parseDescription(*this, tlib);
    for (UINT index = 0; index < tlib.count(); ++index) {
        auto info = tlib.info(index);
        auto name = info.documentation(-1).name;

        // get items
        auto lib = info.typelib();
        if (lib != tlib) {
            // never seen an external symbol before
            assert(false);
        } else {
            detail::parseItem(description, info);
        }
    }
}


}   /* autocom */
