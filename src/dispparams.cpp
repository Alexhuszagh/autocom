//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief Templates to automate dispparams construction.
 */

#include "autocom/dispparams.hpp"


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


/** \brief Reset DISPPARAMS.
 */
void DispParams::reset(const bool useNamed)
{
    dp.cArgs = vargs.size();
    if (vargs.size()) {
        dp.rgvarg = const_cast<Variant*>(vargs.data());
    }

    if (useNamed) {
        dp.cNamedArgs = 1;
        dp.rgdispidNamedArgs = &named;
    } else {
        dp.cNamedArgs = 0;
        dp.rgdispidNamedArgs = nullptr;
    }
}


/** \brief Null constructor.
 */
DispParams::DispParams()
{
    reset(false);
}


/** \brief Copy constructor.
 */
DispParams::DispParams(const DispParams &other):
    vargs(other.vargs)
{
    reset(other.dp.rgdispidNamedArgs);
}


/** \brief Copy assignment operator.
 */
DispParams & DispParams::operator=(const DispParams &other)
{
    vargs = other.vargs;
    reset(other.dp.rgdispidNamedArgs);

    return *this;
}


/** \brief Move constructor.
 */
DispParams::DispParams(DispParams &&other):
    vargs(std::move(other.vargs))
{
    reset(std::move(other.dp.rgdispidNamedArgs));
}


/** \brief Move assignment operator.
 */
DispParams & DispParams::operator=(DispParams &&other)
{
    vargs = std::move(other.vargs);
    reset(std::move(other.dp.rgdispidNamedArgs));

    return *this;
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
