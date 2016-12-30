//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCom
 *  \brief COM exception definitions.
 */

#include "autocom.hpp"


namespace autocom
{
// EXCEPTION
// ---------


/** \brief Construct exception with error.
 */
ComFunctionError::ComFunctionError(const std::string &function):
    message("AutoCom: Unable to call function: " + function + ".")
{}


/** \brief Construct exception with error.
 */
ComMethodError::ComMethodError(const std::string &object,
        const std::string &method):
    message("AutoCom: Unable to call method: " + object + "::" + method + ".")
{}


}   /* autocom */
