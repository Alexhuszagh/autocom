//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCom
 *  \brief Variant object and collection definitions.
 */

#include "autocom.hpp"


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

}   /* autocom */
