//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup AutoCom
 *  \brief Enum utilities.
 */

#pragma once

#include <type_traits>


namespace autocom
{
// FUNCTIONS
// ---------


/** \brief Convert enum to underlying type.
 */
template <typename Enum>
auto FROM_ENUM(const Enum value)
    -> typename std::underlying_type<Enum>::type
{
    return static_cast<typename std::underlying_type<Enum>::type>(value);
}


/** \brief Macro to define enum operators.
 *
 *  Supports `&`, `&=`, `|`, `|=`, `^`, `^=`, `~`, and bool conversion.
 */
#define AUTOCOM_FLAGS(Enum)                                             \
    /*  \brief Bitwise or operator. */                                  \
    inline Enum operator|(const Enum left, const Enum right)            \
    {                                                                   \
        return static_cast<Enum>(FROM_ENUM(left) | FROM_ENUM(right));   \
    }                                                                   \
                                                                        \
    /*  \brief Bitwise or assignment operator. */                       \
    inline Enum & operator|=(Enum &left, const Enum right)              \
    {                                                                   \
        left = static_cast<Enum>(FROM_ENUM(left) | FROM_ENUM(right));   \
        return left;                                                    \
    }                                                                   \
                                                                        \
    /*  \brief Bitwise and operator. */                                 \
    inline Enum operator&(const Enum left, const Enum right)            \
    {                                                                   \
        return static_cast<Enum>(FROM_ENUM(left) & FROM_ENUM(right));   \
    }                                                                   \
                                                                        \
    /*  \brief Bitwise and assignment operator. */                      \
    inline Enum & operator&=(Enum &left, const Enum right)              \
    {                                                                   \
        left = static_cast<Enum>(FROM_ENUM(left) & FROM_ENUM(right));   \
        return left;                                                    \
    }                                                                   \
                                                                        \
    /*  \brief Bitwise xor operator. */                                 \
    inline Enum operator^(const Enum left, const Enum right)            \
    {                                                                   \
        return static_cast<Enum>(FROM_ENUM(left) ^ FROM_ENUM(right));   \
    }                                                                   \
                                                                        \
    /*  \brief Bitwise xor assignment operator. */                      \
    inline Enum & operator^=(Enum &left, const Enum right)              \
    {                                                                   \
        left = static_cast<Enum>(FROM_ENUM(left) ^ FROM_ENUM(right));   \
        return left;                                                    \
    }                                                                   \
                                                                        \
    /*  \brief Bitwise negation operator. */                            \
    inline Enum operator~(const Enum value)                             \
    {                                                                   \
        return static_cast<Enum>(~FROM_ENUM(value));                    \
    }                                                                   \
                                                                        \
    /*  \brief Negation operator. */                                    \
    inline bool operator!(const Enum value)                             \
    {                                                                   \
        return !bool(FROM_ENUM(value));                                 \
    }


}   /* autocom */
