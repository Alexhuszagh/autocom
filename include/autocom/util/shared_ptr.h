//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief Custom shared_ptr for COM objects.
 */

#pragma once

#include <memory>


namespace autocom
{
// FUNCTIONS
// ---------


/** \brief Release COM object.
 */
template <typename T>
void release(T *t)
{
    if (t) {
        t->Release();
    }
}

// OBJECTS
// -------


/** \brief Shared pointer for wrapping COM objects.
 */
template <typename T>
class SharedPointer: public std::shared_ptr<T>
{
protected:
    typedef std::shared_ptr<T> Base;
    typedef SharedPointer<T> This;

    using Base::reset;

public:
    constexpr SharedPointer() noexcept;
    SharedPointer(const SharedPointer&) = default;
    This & operator=(const SharedPointer&) = default;
    SharedPointer(SharedPointer&&) = default;
    This & operator=(SharedPointer&&) = default;

    constexpr SharedPointer(std::nullptr_t nullp) noexcept;
    SharedPointer(T *t);

    void reset();
    void reset(std::nullptr_t nullp);
    void reset(T *t);
};


// IMPLEMENTATION
// --------------


/** \brief Initialize from empty pointer.
 */
template <typename T>
constexpr SharedPointer<T>::SharedPointer() noexcept
{}


/** \brief Initialize from null pointer.
 */
template <typename T>
constexpr SharedPointer<T>::SharedPointer(std::nullptr_t nullp) noexcept
{}


/** \brief Initialize from COM pointer.
 */
template <typename T>
SharedPointer<T>::SharedPointer(T *t):
    Base(t, release<T>)
{}


/** \brief Reset from empty pointer.
 */
template <typename T>
void SharedPointer<T>::reset()
{
    Base::reset();
}


/** \brief Reset from null pointer.
 */
template <typename T>
void SharedPointer<T>::reset(std::nullptr_t nullp)
{
    Base::reset(nullp);
}


/** \brief Reset from COM pointer.
 */
template <typename T>
void SharedPointer<T>::reset(T *t)
{
    Base::reset(t, release<T>);
}

}   /* autocom */
