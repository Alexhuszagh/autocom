//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief COM exception definitions.
 */

#include <autocom/util/exception.h>


namespace autocom
{
// EXCEPTION
// ---------


/** \brief Construct exception with message.
 */
ComFunctionError::ComFunctionError(const std::string &function):
    message("AutoCOM: Unable to call function: " + function + ".")
{}


/** \brief Construct exception with message.
 */
ComMethodError::ComMethodError(const std::string &object,
        const std::string &method):
    message("AutoCOM: Unable to call method: " + object + "::" + method + ".")
{}


/** \brief Construct exception with message.
 */
ComTypeError::ComTypeError(const std::string &expected,
        const std::string &actual,
        const std::string &op):
    message("AutoCOM: Expected \"" + expected + "\" " + op + " " + actual + " to be true.")
{}


}   /* autocom */
