//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCom
 *  \brief Parse ITypeInfo data attributes.
 */

#include "parser.hpp"

#include <cassert>
#include <iostream>             // TODO: REMOVE
#include <sstream>
#include <stdexcept>
#include <unordered_map>


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
};

std::unordered_map<CALLCONV, std::string> DECORATIONS = {
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
void parseItem(Description &description,
    const TypeInfo &info)
{
    auto attr = info.attr();
    switch (attr.kind()) {
        case TKIND_ENUM:
            description.enums.emplace_back(Enum(info, attr));
            break;
        case TKIND_RECORD:
            description.records.emplace_back(Record(info, attr));
            break;
        case TKIND_MODULE:
            description.modules.emplace_back(Module(info, attr));
            break;
        case TKIND_INTERFACE:
            description.interfaces.emplace_back(Interface(info, attr));
            break;
        case TKIND_DISPATCH:
            description.dispatchers.emplace_back(Dispatch(info, attr));
            break;
        case TKIND_COCLASS:
            description.coclasses.emplace_back(CoClass(info, attr));
            break;
        case TKIND_ALIAS:
            description.aliases.emplace_back(Alias(info, attr));
            break;
        case TKIND_UNION:
            description.unions.emplace_back(Union(info, attr));
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
        // get safearray
        auto name = getTypeName(info, desc.pointer());
        printf("ERROR: getTypeName(VT_SAFEARRAY)\n");
        assert(false);
    }

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


/** \brief Get source definition.
 */
std::string CppCode::source() const
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


/** \brief Get representation in source.
 */
std::string Variable::source() const
{
    return type + " " + name + " = " + value;
}


/** \brief Parse function definition.
 */
Function::Function(const TypeInfo &info,
    const WORD index)
{
    // get descriptors
    auto fd = info.funcdesc(index);
    auto documentation = info.documentation(fd.id());
    assert(fd.kind() == FUNC_PUREVIRTUAL);

    decorator = DECORATIONS[fd.decoration()];
    returns = getTypeName(info, fd.returnType().type());
    name = documentation.name;
    doc = documentation.doc;
    id = fd.id();
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


/** \brief Get body of function definition.
 */
std::string Function::body() const
{
    return "";
    // TODO:
}


/** \brief Get representation in header.
 */
std::string Function::header() const
{
    std::ostringstream stream;
    stream << decorator << " " << returns << " " << definition() << ";";

    return stream.str();
}


/** \brief Get representation in source.
 */
std::string Function::source() const
{
    return "";
    // TODO:
}


/** \brief Initialize Enum method description from TypeInfo.
 */
Enum::Enum(const TypeInfo &info,
    const TypeAttr &attr)
{
    name = info.documentation(-1).name;
    for (WORD index = 0; index < attr.variables(); ++index) {
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
    const TypeAttr &attr)
{
    documentation = info.documentation(-1);
    size = attr.size();
    for (WORD index = 0; index < attr.variables(); ++index) {
        fields.emplace_back(Parameter(info, index));
    }
}


/** \brief Forward declaration for struct.
 */
std::string Record::forward() const
{
    return "struct " + documentation.name + ";";
};


/** \brief Write struct to string for header.
 */
std::string Record::header() const
{
    std::ostringstream stream;
    stream << "struct " << documentation.name << "\r\n"
           << "{\r\n";
    for (const auto &field: fields) {
        stream << "    " << field.header() << ";\r\n";
    }
    stream << "};\r\n";
    stream << "static_assert(sizeof(" << documentation.name
           << ") == " << size
           << ", \"AutoCom: Invalid struct size.\");\r\n";

    return stream.str();
}


/** \brief Initialize Module method description from TypeInfo.
 *
 *  \warning This code should be assumed to be buggy, since no COM
 *  DLLs installed on Wine use TKIND_MODULE.
 */
Module::Module(const TypeInfo &info,
    const TypeAttr &attr)
{
    // functions
    for (WORD index = 0; index < attr.functions(); ++index) {
        functions.emplace_back(Function(info, index));
    }

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


/** \brief Write module to string for source.
 */
std::string Module::source() const
{
    std::ostringstream stream;
    for (const auto &item: functions) {
        stream << item.source() << "\r\n";
    }
    for (const auto &item: constants) {
        stream << "const " << item.source() << ";\r\n";
    }

    return stream.str();
}


/** \brief Initialize Interface method description from TypeInfo.
 */
Interface::Interface(const TypeInfo &info,
    const TypeAttr &attr)
{
    documentation = info.documentation(-1);
    iid = attr.guid();
    flags = attr.flags();

    if (attr.interfaces()) {
        base = info.info(info.reference(0)).documentation(-1).name;
    }

    assert(attr.variables() == 0);
    for (WORD index = 0; index < attr.functions(); ++index) {
        functions.emplace_back(Function(info, index));
    }
}


/** \brief Write interface to string for header.
 */
std::string Interface::header() const
{
    std::ostringstream stream;

    // class definition and base class
    stream << "struct " << documentation.name;
    if (!base.empty()) {
        stream << ": " << base;
    }
    // initialize with static, constexpr values which do not add to struct
    stream << "\r\n"
          << "{\r\n"
           << "    static constexpr char iid[] = \""
           << iid.string() << "\";\r\n"
           << "    static constexpr WORD flags = " << flags << ";\r\n";
    // functions
    // TODO: need to check inheritance...
//    if (base == "IUnknown") {
//        // comment out methods without Invoke
//        for (const auto &item: functions) {
//            stream << "    // " << item.header() << "\r\n";
//        }
//    } else {
//        for (const auto &item: functions) {
//            stream << "    " << item.header() << "\r\n";
//        }
//    }

    stream << "};\r\n";

    return stream.str();
}


/** \brief Write interface to string for header.
 */
std::string Interface::source() const
{
    std::ostringstream stream;
//    if (base != "IUnknown") {
//        for (const auto &item: functions) {
//            stream << item.returns << " " << documentation.name
//                    << "::" << item.body() << "\r\n\r\n";
//        }
//    }

    return stream.str();
}


/** \brief Initialize Dispatch method description from TypeInfo.
 */
Dispatch::Dispatch(const TypeInfo &info,
    const TypeAttr &attr)
{
    std::cout << "------------------" << std::endl;
    std::cout << "Parsing Dispatch" << std::endl;
}


/** \brief Write dispatcher to string for header.
 */
std::string Dispatch::header() const
{
    return "";
}


/** \brief Initialize CoClass method description from TypeInfo.
 */
CoClass::CoClass(const TypeInfo &info,
    const TypeAttr &attr)
{
    std::cout << "------------------" << std::endl;
    std::cout << "Parsing CoClass" << std::endl;
}


/** \brief Write coclass to string for header.
 */
std::string CoClass::header() const
{
    return "";
}


/** \brief Initialize Alias method description from TypeInfo.
 */
Alias::Alias(const TypeInfo &info,
    const TypeAttr &attr)
{
    std::cout << "------------------" << std::endl;
    std::cout << "Parsing Alias" << std::endl;
}


/** \brief Write alias to string for header.
 */
std::string Alias::header() const
{
    return "";
}


/** \brief Initialize Union method description from TypeInfo.
 */
Union::Union(const TypeInfo &info,
    const TypeAttr &attr)
{
    std::cout << "------------------" << std::endl;
    std::cout << "Parsing Union" << std::endl;
}


/** \brief Write union to string for header.
 */
std::string Union::header() const
{
    return "";
}


/** \brief Write external library to string for header.
 */
std::string External::header() const
{
    return "";
}


}   /* detail */


// PARSER
// ------


/** \brief Parse TypeLib from COM object.
 */
void TypeLibDescription::parse(const TypeLib &tlib)
{
    // TODO: need a memo to check for redundant items

    // get library definitions
    detail::parseDescription(*this, tlib);
    for (UINT index = 0; index < tlib.count(); ++index) {
        auto info = tlib.info(index);
        auto name = info.documentation(-1).name;
        // TODO: redundancy check...
        // COMTYPES, C++
        // modname = self._typelib_module()
        // auto modname = std::string(tlib.attr());
//        std::cout << "--------------------------------" << std::endl;
//        std::cout << "Index is " << index << std::endl;
//        std::cout << "Name is " << name << std::endl;
//        std::cout << "--------------------------------" << std::endl;

        // get child libraries..
        auto lib = info.typelib();
        if (lib != tlib) {
            //description.emplace_back(External());
//            std::cout << "--------------------------------" << std::endl;
//            std::cout << "Have external depndency" << std::endl;
//            std::cout << "--------------------------------" << std::endl;
        } else {
            detail::parseItem(description, info);
        }
    }

//    std::cout << "--------------------------------" << std::endl;
//    std::cout << guid.toClsid() << std::endl;
//    std::cout << major << std::endl;
//    std::cout << minor << std::endl;
//    std::cout << documentation.name << std::endl;
//    std::cout << documentation.doc << std::endl;
//    std::cout << documentation.help << std::endl;
//    std::cout << documentation.file << std::endl;
//    std::cout << "--------------------------------" << std::endl;
}


}   /* autocom */
