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


/** \brief Dispparams type error.
 */
class DispparamsTypeError: public std::exception
{
    virtual const char *what() const throw()
    {
        return "Cannot add value to DISPPARAMS: type not recognized.\n";
    }
};


}   /* autocom */
