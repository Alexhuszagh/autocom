//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCom
 *  \brief Parse ITypeInfo data attributes.
 */

#include "parser.hpp"

#include <iostream>             // TODO: REMOVE


namespace autocom
{
namespace detail
{
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

// OBJECTS
// -------


/** \brief Initialize Enum method description from TypeInfo.
 */
Enum::Enum(const TypeInfo &info,
    const TypeAttr &attr)
{
    name = info.documentation(-1).name;
    std::cout << "--------------------------------" << std::endl;
    std::cout << name << std::endl;
    for (WORD i = 0; i < attr.variables(); ++i) {
        std::cout << i << std::endl;
        // VARDESC
        // GetVarDesc
        // ReleaseVarDesc
    }

    std::cout << "--------------------------------" << std::endl;
}


/** \brief Initialize Record method description from TypeInfo.
 */
Record::Record(const TypeInfo &info,
    const TypeAttr &attr)
{}


/** \brief Initialize Module method description from TypeInfo.
 */
Module::Module(const TypeInfo &info,
    const TypeAttr &attr)
{}


/** \brief Initialize Interface method description from TypeInfo.
 */
Interface::Interface(const TypeInfo &info,
    const TypeAttr &attr)
{}


/** \brief Initialize Dispatch method description from TypeInfo.
 */
Dispatch::Dispatch(const TypeInfo &info,
    const TypeAttr &attr)
{}


/** \brief Initialize CoClass method description from TypeInfo.
 */
CoClass::CoClass(const TypeInfo &info,
    const TypeAttr &attr)
{}


/** \brief Initialize Alias method description from TypeInfo.
 */
Alias::Alias(const TypeInfo &info,
    const TypeAttr &attr)
{}


/** \brief Initialize Union method description from TypeInfo.
 */
Union::Union(const TypeInfo &info,
    const TypeAttr &attr)
{}


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
