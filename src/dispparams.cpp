//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCom
 *  \brief Templates to automate dispparams construction.
 */

#include "autocom.hpp"


namespace autocom
{
// OBJECTS
// -------


/** \brief Null constructor.
 */
DispParams::DispParams()
{
    params.rgvarg = const_cast<VARIANT*>(args.data());
    params.cArgs = 0;
    params.rgdispidNamedArgs = nullptr;
    params.cNamedArgs = 0;
}


/** \brief Copy constructor.
 */
DispParams::DispParams(const DispParams &other):
    args(other.args),
    named(other.named)
{
    params.cArgs = args.size();
    params.rgvarg = const_cast<VARIANT*>(args.data());
    params.cNamedArgs = other.params.cNamedArgs;
    if (other.params.rgdispidNamedArgs) {
        params.rgdispidNamedArgs = &named;
    } else {
        params.rgdispidNamedArgs = nullptr;
    }
}


/** \brief Destructor.
 */
DispParams::~DispParams()
{}


/** \brief Set dispatch method, altering the named argument count.
 */
void DispParams::setDispatch(const Dispatch dispatch)
{
    if (!!(dispatch & (Dispatch::PROPERTYPUT | Dispatch::PROPERTYPUTREF))) {
        params.cNamedArgs = 1;
        params.rgdispidNamedArgs = &named;
    }
}


}   /* autocom */
