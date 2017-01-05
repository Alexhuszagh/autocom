//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief C++ handle around IDispatch.
 */

#include "autocom/dispatch.hpp"


namespace autocom
{
// OBJECTS
// -------


/** \brief Null constructor.
 */
Dispatch::Dispatch()
{
    initialize();
}


/** \brief Copy constructor.
 */
Dispatch::Dispatch(const Dispatch &other):
    DispatchBase(other)
{
    initialize();
}


Dispatch & Dispatch::operator=(const Dispatch &other)
{
    initialize();
    DispatchBase::operator=(other);
    return *this;
}


/** \brief Move constructor.
 */
Dispatch::Dispatch(Dispatch &&other):
    DispatchBase(std::move(other))
{
    initialize();
}


Dispatch & Dispatch::operator=(Dispatch &&other)
{
    initialize();
    DispatchBase::operator=(std::move(other));
    return *this;
}


/** \brief Destructor.
 */
Dispatch::~Dispatch()
{
    uninitialize();
}


/** \brief Construct dispatcher from GUID.
 */
Dispatch::Dispatch(const Guid &guid,
    LPUNKNOWN outter,
    DWORD context)
{
    initialize();
    open(guid, outter, context);
}


/** \brief Open handle to IDispatch COM object.
 */
void Dispatch::open(const Guid &guid,
    LPUNKNOWN outter,
    DWORD context)
{
    IDispatch *dispatch;
    if (FAILED(CoCreateInstance(guid.id, outter, context, IID_IDispatch, (void **) &dispatch))) {
        throw ComFunctionError("CoCreateInstance()");
    }
    DispatchBase::ppv.reset(dispatch);
}


/** \brief Construct ITypeInfo from IDispatch.
 */
TypeInfo Dispatch::info() const
{
    return TypeInfo(newTypeInfo(DispatchBase::ppv.get()));
}


}   /* autocom */
