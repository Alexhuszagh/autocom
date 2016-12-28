//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCom
 *  \brief COM exception definitions.
 */

#pragma once

#include <stdexcept>
#include <typeinfo>


namespace autocom
{
// EXCEPTION
// ---------


/** \brief Unable to create COM object instance.
 */
class CoCreateInstanceError: public std::exception
{
    virtual const char *what() const throw()
    {
        return "Unable to call CoCreateInstance.\n";
    }
};


/** \brief Unable to get function member ID.
 */
class GetIDsOfNamesError: public std::exception
{
    virtual const char *what() const throw()
    {
        return "Unable to call GetIDsOfNames.\n";
    }
};


/** \brief Dispparams type error.
 */
class DispparamsTypeError: public std::exception
{
    virtual const char *what() const throw()
    {
        return "Cannot add value to DISPPARAMS: type not recognized.\n";
    }
};


/** \brief Unable to dispatch DISPID_NEWENUM for new IEnumVARIANT.
 */
class IEnumVariantInvokeError: public std::exception
{
    virtual const char *what() const throw()
    {
        return "Unable to Invoke(DISPID_NEWENUM, ...) to get IEnumVARIANT.\n";
    }
};


/** \brief Unable to query interface of IEnumVARIANT.
 */
class IEnumVariantQueryError: public std::exception
{
    virtual const char *what() const throw()
    {
        return "Unable to QueryInterface(IID_IEnumVARIANT) for IEnumVARIANT.\n";
    }
};


/** \brief Unable to cast variant to desired type.
 */
class VariantGetValueError: public std::exception
{
    virtual const char *what() const throw()
    {
        return "Unable to cast value to desired type using VariantChangeType.\n";
    }
};


}   /* autocom */