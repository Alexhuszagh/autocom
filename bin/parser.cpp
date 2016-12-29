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
std::string getTypeName(const TypeDesc &desc,
    const TypeInfo &info)
{
    auto it = TYPE_NAMES.find(desc.vt());
    if (it != TYPE_NAMES.end()) {
        return it->second;
    } else if (desc.vt() == VT_CARRAY) {
        // get type description for C-style array: char[5][4]
        auto array = desc.array();
        auto name = getTypeName(array.type(), info);
        for (USHORT index = 0; index < array.count(); ++index) {
            name += "[" + std::to_string(array.bound(index).size()) + "]";
        }
        return name;
    } else if (desc.vt() == VT_PTR) {
        // return pointer type
        return getTypeName(desc.pointer(), info) + "*";
    } else if (desc.vt() == VT_USERDEFINED) {
        return info.info(desc.reference()).documentation(-1).name;
    } else if (desc.vt() == VT_SAFEARRAY) {
        // get safearray
        auto name = getTypeName(desc.pointer(), info);
        printf("ERROR: getTypeName(VT_SAFEARRAY)\n");
        assert(false);
    }

    throw std::invalid_argument("Invalid type: " + std::to_string(desc.vt()));
}


// OBJECTS
// -------


/** \brief Initialize Enum method description from TypeInfo.
 */
Enum::Enum(const TypeInfo &info,
    const TypeAttr &attr)
{
    name = info.documentation(-1).name;
    for (WORD index = 0; index < attr.variables(); ++index) {
        // get descriptors
        auto vd = info.vardesc(index);
        auto doc = info.documentation(vd.id());
        auto variant = vd.variant();

        // enum underlying type
        VariantChangeType(&variant, &variant, 0, VT_I8);
        values.emplace_back(std::make_tuple(doc.name, variant.llVal));
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
        stream << "    " << std::get<0>(value) << " = " << std::get<1>(value) << ",\r\n";
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
        // get descriptors
        auto vd = info.vardesc(index);
        auto doc = info.documentation(vd.id());
        assert(vd.kind() == VAR_PERINSTANCE);
        auto type = getTypeName(vd.element().type(), info);
        fields.emplace_back(std::make_tuple(type, doc.name));
    }
//
//    std::cout << "--------------------------\r\n"
//              << forward() << "\r\n"
//              << header() << "\r\n";
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
        stream << "    " << std::get<0>(field) << " " << std::get<1>(field) << ";\r\n";
    }
    stream << "};\r\n";
    stream << "static_assert(sizeof(" << documentation.name
           << ") == " << size
           << ", \"AutoCom: Invalid struct size.\");\r\n";

    return stream.str();
}


/** \brief Initialize Module method description from TypeInfo.
 */
Module::Module(const TypeInfo &info,
    const TypeAttr &attr)
{
    // TODO: this is poorly tested
    std::cout << "------------------" << std::endl;
    std::cout << "Parsing module" << std::endl;
}


/** \brief Write module to string for header.
 */
std::string Module::header() const
{
    return "";
}


/** \brief Initialize Interface method description from TypeInfo.
 */
Interface::Interface(const TypeInfo &info,
    const TypeAttr &attr)
{
    std::cout << "------------------" << std::endl;
    std::cout << "Parsing interface" << std::endl;
}


/** \brief Write interface to string for header.
 */
std::string Interface::header() const
{
    return "";
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
