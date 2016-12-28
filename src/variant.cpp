//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCom
 *  \brief Variant object and collection definitions.
 */

#include "autocom.hpp"

#ifdef HAVE_PROPSYS
#   include <propvarutil.h>
#endif      // HAVE_PROPSYS


namespace autocom
{
// FUNCTIONS
// ---------


/** \brief Convert VARIANT data to new type.
 */
bool changeVariantType(VARIANT &variant,
    const VARTYPE vt)
{
    return VariantChangeType(&variant, &variant, 0, vt) == S_OK;
}


#ifdef HAVE_PROPSYS

    /** \brief Convert PROPVARIANT data to new type.
     *
     *  Since the source and destination buffer cannot be modified in-place,
     *  since PropVariantChangeType would destroy the input buffer,
     *  created a temporary PropVariant (which is automatically cleared
     *  on leaving scope), and swap the input and output buffers only
     *  if the type-change is successful.
     */
    bool changeVariantType(PROPVARIANT &input,
        const VARTYPE vt)
    {
        PropVariant output;
        const bool status = PropVariantChangeType(&input, output, 0, vt) == S_OK;
        if (status) {
            std::swap(input, static_cast<PROPVARIANT&>(output));
            return true;
        }

        return false;
    }

#endif          // HAVE_PROPSYS


// OBJECTS
// -------


/** \brief Null constructor.
 */
Variant::Variant()
{
    init();
}


/** \brief Destructor.
 */
Variant::~Variant()
{
    clear();
}


/** \brief Initialize variant.
 */
void Variant::init()
{
    VariantInit(this);
}


/** \brief Clear variant.
 */
void Variant::clear()
{
    VariantClear(this);
}


/** \brief Coerce variant type to specified type.
 */
bool Variant::changeType(const VARTYPE vt)
{
    return changeVariantType(*this, vt);
}


#ifdef HAVE_PROPSYS

    /** \brief Null constructor.
     */
    PropVariant::PropVariant()
    {
        init();
    }


    /** \brief Destructor.
     */
    PropVariant::~PropVariant()
    {
        clear();
    }


    /** \brief Initialize property variant.
     */
    void PropVariant::init()
    {
        PropVariantInit(this);
    }


    /** \brief Clear property variant.
     */
    void PropVariant::clear()
    {
        PropVariantClear(this);
    }


    /** \brief Coerce variant type to specified type.
     *
     *  \return             Type coercion was successful
     */
    bool PropVariant::changeType(const VARTYPE vt)
    {
        return changeVariantType(*this, vt);
    }

#endif          // HAVE_PROPSYS


}   /* autocom */
