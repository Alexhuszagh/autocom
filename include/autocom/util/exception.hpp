//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCom
 *  \brief COM exception definitions.
 */

#pragma once

#include <stdexcept>
#include <string>
#include <typeinfo>


namespace autocom
{
// EXCEPTION
// ---------


/** \brief Wraps a general COM method error.
 */
class ComFunctionError: public std::exception
{
protected:
    std::string message;

    virtual const char *what() const throw()
    {
        return message.data();
    }

public:
    ComFunctionError(const std::string &function);
};


/** \brief Wraps a general COM method error.
 */
class ComMethodError: public std::exception
{
protected:
    std::string message;

    virtual const char *what() const throw()
    {
        return message.data();
    }

public:
    ComMethodError(const std::string &object,
        const std::string &method);
};

}   /* autocom */
