//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief Templates to automate dispparams construction.
 */

#include "autocom.hpp"


namespace autocom
{
// CONSTANTS
// ---------

const DispatchFlags GET = DispatchFlags::GET | DispatchFlags::METHOD;
const DispatchFlags METHOD = DispatchFlags::METHOD;
const DispatchFlags PUT = DispatchFlags::PUT;
const DispatchFlags PUTREF = DispatchFlags::PUTREF;

// OBJECTS
// -------


/** \brief Null constructor.
 */
DispParams::DispParams()
{
    dp.cArgs = 0;
    dp.rgdispidNamedArgs = nullptr;
    dp.cNamedArgs = 0;
}


/** \brief Copy constructor.
 */
DispParams::DispParams(const DispParams &other):
    vargs(other.vargs),
    named(other.named)
{
    dp.cArgs = vargs.size();
    dp.rgvarg = const_cast<Variant*>(vargs.data());
    dp.cNamedArgs = other.dp.cNamedArgs;
    if (other.dp.rgdispidNamedArgs) {
        dp.rgdispidNamedArgs = &named;
    } else {
        dp.rgdispidNamedArgs = nullptr;
    }
}


/** \brief Destructor.
 */
DispParams::~DispParams()
{}


/** \brief Set dispatch method, altering the named argument count.
 */
void DispParams::setFlags(const DispatchFlags flags)
{
    if (!!(flags & (PUT | PUTREF))) {
        dp.cNamedArgs = 1;
        dp.rgdispidNamedArgs = &named;
    }
}


/** \brief Get access to raw dispparams.
 */
DISPPARAMS * DispParams::params()
{
    return &dp;
}


/** \brief Get access to raw dispparams.
 */
const DISPPARAMS * DispParams::params() const
{
    return &dp;
}


/** \brief Get access to raw arg array.
 */
const VariantList & DispParams::args() const
{
    return vargs;
}


}   /* autocom */
