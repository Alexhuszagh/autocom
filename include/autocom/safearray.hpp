//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief COM SafeArray wrapper.
 */

#pragma once

#include "util.hpp"

#include <oaidl.h>

#include <vector>


namespace autocom
{
// FUNCTIONS
// ---------

/** \brief IUnknown type-safe wrapper for SafeArrayGetVartype.
 *
 *  SafeArrayGetVartype fails for IUnknown, because MSDN cannot properly
 *  define flags, or do bitwise operations, so must check explicitly.
 *  https://msdn.microsoft.com/en-us/library/windows/desktop/ms221446(v=vs.85).aspx
 */
VARTYPE getSafeArrayType(const SAFEARRAY *value);

// OBJECTS
// -------


/** \brief C++ wrapper around SAFEARRAYBOUND.
 */
struct SafeArrayBound: SAFEARRAYBOUND
{
    SafeArrayBound() = default;
    SafeArrayBound(const SafeArrayBound&) = default;
    SafeArrayBound & operator=(const SafeArrayBound&) = default;
    SafeArrayBound(SafeArrayBound&&) = default;
    SafeArrayBound & operator=(SafeArrayBound&&) = default;

    SafeArrayBound(const SAFEARRAYBOUND &bound);
    SafeArrayBound(SAFEARRAYBOUND &&bound);
    SafeArrayBound(const size_t size);
    SafeArrayBound(const size_t lower,
        const size_t upper);

    // DATA
    ULONG size() const;
    LONG & lower();
    ULONG & upper();
};


// TODO: need iterators...


/** \brief C++ wrapper around SAFEARRAY.
 *
 *  Provides an STL-like interface with automatic std::vector
 *  conversions.
 */
template <typename T>
class SafeArray: public SAFEARRAY
{
protected:
    typedef SafeArray<T> This;

    void lock();
    void unlock();

    // INITIALIZERS
    void create(UINT dimensions,
        SafeArrayBound *bound);
    void close();

public:
    // MEMBER TYPES
    // ------------
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef pointer iterator;
    typedef const_pointer const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    static constexpr VARTYPE vt = VariantType<T, true>::vt;

    SafeArray();
    ~SafeArray();
    SafeArray(const This &other);
    This & operator=(const This &other);
    SafeArray(This&&) = default;
    SafeArray & operator=(This&&) = default;

    SafeArray(const SAFEARRAY &other);
    This & operator=(const SAFEARRAY &other);
    SafeArray(SAFEARRAY &&other);

    // CAPACITY
    size_t size(const LONG size = -1) const;
    bool empty() const;

    // ITERATORS
    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    reverse_iterator rbegin() noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;

    // ELEMENT ACCESS
    reference operator[](LONG *indices);
    const_reference operator[](LONG *indices) const;
    reference at(LONG *indices);
    const_reference at(LONG *indices) const;
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;
    const_pointer data() const;

    // MODIFIERS
    void resize(SafeArrayBound *bound);
    void resize(const LONG size);

    // DATA
    This copy() const;
    explicit operator std::vector<T>() const;
};

// DOWNCASTING
// -----------

static_assert(sizeof(SafeArrayBound) == sizeof(SAFEARRAYBOUND), "sizeof(SafeArrayBound) != sizeof(SAFEARRAYBOUND), cannot safely downcast");
static_assert(sizeof(SafeArray<INT>) == sizeof(SAFEARRAY), "sizeof(SafeArray) != sizeof(SAFEARRAY), cannot safely downcast");

// IMPLEMENTATION
// --------------


/** \brief Lock array, forcing it to take a take a fixed memory location.
 */
template <typename T>
void SafeArray<T>::lock()
{
    if (FAILED(SafeArrayLock(this))) {
        throw ComFunctionError("SafeArrayLock()");
    }
}


/** \brief Ulock array.
 */
template <typename T>
void SafeArray<T>::unlock()
{
    if (FAILED(SafeArrayUnlock(this))) {
        throw ComFunctionError("SafeArrayUnlock()");
    }
}


/** \brief Create array.
 */
template <typename T>
void SafeArray<T>::create(UINT dimensions,
    SafeArrayBound *bound)
{

    auto *array = SafeArrayCreate(vt, dimensions, (bound));
    if (!array) {
        throw std::runtime_error("Unhandled exception in SafeArrayCreate, maybe out of memory?\n");
    }
    reinterpret_cast<SAFEARRAY&>(*this) = *array;
}


/** \brief Destroy array.
 */
template <typename T>
void SafeArray<T>::close()
{
    unlock();
    SafeArrayDestroy(this);
}


/** \brief Create empty array.
 */
template <typename T>
SafeArray<T>::SafeArray()
{
    SafeArrayBound bound(0);
    create(1, &bound);
    lock();
}


/** \brief Copy constructor.
 */
template <typename T>
SafeArray<T>::SafeArray(const This &other)
{
    operator=(other);
}


/** \brief Copy assignment operator.
 */
template <typename T>
auto SafeArray<T>::operator=(const This &other)
    -> This &
{
    SAFEARRAY *array;
    auto *ptr = reinterpret_cast<const SAFEARRAY*>(&other);
    if (SafeArrayCopy(ptr, &array) == E_OUTOFMEMORY) {
        throw std::runtime_error("E_OUTOFMEMORY from SafeArrayCopy()\n");
    }
    reinterpret_cast<SAFEARRAY&>(*this) = *array;
    lock();

    return *this;
}


/** \brief Copy array.
 */
template <typename T>
SafeArray<T>::SafeArray(const SAFEARRAY &other)
{
    operator=(other);
}


/** \brief Copy array.
 */
template <typename T>
auto SafeArray<T>::operator=(const SAFEARRAY &other)
    -> This &
{
    if (vt != getSafeArrayType(&other)) {
        throw std::invalid_argument("Cannot change type of SafeArray");
    }
    SAFEARRAY *array;
    if (SafeArrayCopy(&other, &array) == E_OUTOFMEMORY) {
        throw std::runtime_error("E_OUTOFMEMORY from SafeArrayCopy()\n");
    }
    reinterpret_cast<SAFEARRAY&>(*this) = *array;
    lock();

    return *this;
}


/** \brief Move array.
 */
template <typename T>
SafeArray<T>::SafeArray(SAFEARRAY &&other):
    SAFEARRAY(std::move(other))
{
    lock();
}


/** \brief Destructor.
 */
template <typename T>
SafeArray<T>::~SafeArray()
{
    close();
}


/** \brief Get iterator at beginning of array.
 */
template <typename T>
auto SafeArray<T>::begin() noexcept
    -> iterator
{
    return SAFEARRAY::pvData;
}


/** \brief Get iterator past end of array.
 */
template <typename T>
auto SafeArray<T>::end() noexcept
    -> iterator
{
    return begin() + size(-1);
}


/** \brief Get iterator at beginning of array.
 */
template <typename T>
auto SafeArray<T>::begin() const noexcept
    -> const_iterator
{
    return cbegin();
}


/** \brief Get iterator past end of array.
 */
template <typename T>
auto SafeArray<T>::end() const noexcept
    -> const_iterator
{
    return cend();
}


/** \brief Get iterator at beginning of array.
 */
template <typename T>
auto SafeArray<T>::cbegin() const noexcept
    -> const_iterator
{
    return const_cast<const_iterator>(SAFEARRAY::pvData);
}


/** \brief Get iterator past end of array.
 */
template <typename T>
auto SafeArray<T>::cend() const noexcept
    -> const_iterator
{
    return cbegin() + size(-1);
}


/** \brief Get iterator at reverse beginning of array.
 */
template <typename T>
auto SafeArray<T>::rbegin() noexcept
    -> reverse_iterator
{
    return reverse_iterator(end());
}


/** \brief Get iterator past reverse end of array.
 */
template <typename T>
auto SafeArray<T>::rend() noexcept
    -> reverse_iterator
{
    return reverse_iterator(begin());
}


/** \brief Get iterator at reverse beginning of array.
 */
template <typename T>
auto SafeArray<T>::rbegin() const noexcept
    -> const_reverse_iterator
{
    return const_reverse_iterator(cend());
}


/** \brief Get iterator past reverse end of array.
 */
template <typename T>
auto SafeArray<T>::rend() const noexcept
    -> const_reverse_iterator
{
    return const_reverse_iterator(cbegin());
}


/** \brief Get iterator at reverse beginning of array.
 */
template <typename T>
auto SafeArray<T>::crbegin() const noexcept
    -> const_reverse_iterator
{
    return const_reverse_iterator(cend());
}


/** \brief Get iterator past reverse end of array.
 */
template <typename T>
auto SafeArray<T>::crend() const noexcept
    -> const_reverse_iterator
{
    return const_reverse_iterator(cbegin());
}


/** \brief Get size of array.
 */
template <typename T>
size_t SafeArray<T>::size(const LONG size) const
{
    if (SAFEARRAY::cDims < 1 || SAFEARRAY::cDims >= size) {
        throw std::out_of_range("SafeArray:: Size requested is out of bounds");
    }

    if (size == -1) {
        // get all dimensions
        size_t size = 1;
        for (USHORT i = 0; i < SAFEARRAY::cDims; ++i) {
            auto &bound = SAFEARRAY::rgsabound[i];
            size *= (bound.cElements - bound.lLbound);
        }
        return size;
    } else {
        // get single dimension
        auto &bound = SAFEARRAY::rgsabound[size];
        return (bound.cElements - bound.lLbound);
    }
}


/** \brief Check if array is empty.
 */
template <typename T>
bool SafeArray<T>::empty() const
{
    return size(-1) == 0;
}


/** \brief Get element at index.
 */
template <typename T>
auto SafeArray<T>::operator[](LONG *indices)
    -> reference
{
    return at(indices);
}


/** \brief Get element at index.
 */
template <typename T>
auto SafeArray<T>::operator[](LONG *indices) const
    -> const_reference
{
    return at(indices);
}


/** \brief Get element at index.
 */
template <typename T>
auto SafeArray<T>::at(LONG *indices)
    -> reference
{
    T *data;
    SafeArrayPtrOfIndex(this, indices, (void**) &data);

    return *data;
}


/** \brief Get element at index.
 */
template <typename T>
auto SafeArray<T>::at(LONG *indices) const
    -> const_reference
{
    T *data;
    SafeArrayPtrOfIndex(this, indices, (void**) &data);

    return const_cast<const_reference>(data);
}


/** \brief Get first element in array.
 */
template <typename T>
auto SafeArray<T>::front()
    -> reference
{
    return *begin();
}


/** \brief Get first element in array.
 */
template <typename T>
auto SafeArray<T>::front() const
    -> const_reference
{
    return *cbegin();
}


/** \brief Get last element in array.
 */
template <typename T>
auto SafeArray<T>::back()
    -> reference
{
    return *(end() - 1);
}


/** \brief Get last element in array.
 */
template <typename T>
auto SafeArray<T>::back() const
    -> const_reference
{
    return *(cend() - 1);
}


/** \brief Get access to underlying buffer.
 */
template <typename T>
auto SafeArray<T>::data() const
    -> const_pointer
{
    auto *array = reinterpret_cast<pointer>(SAFEARRAY::pvData);
    return const_cast<const_pointer>(array);
}


/** \brief Change dimension bounds with SafeArrayBound.
 *
 *  \warning You can only change the least significant bound.
 */
template <typename T>
void SafeArray<T>::resize(SafeArrayBound *bound)
{
    unlock();
    if (FAILED(SafeArrayRedim(this, bound))) {
        throw ComMethodError("SafeArray", "SafeArrayRedim");
    }

    lock();
}


/** \brief Change dimension bounds size of least significant bound.
 */
template <typename T>
void SafeArray<T>::resize(const LONG size)
{
    // create custom bounds
    SafeArrayBound *bound = new SafeArrayBound[SAFEARRAY::cDims];
    for (USHORT i = 0; i < SAFEARRAY::cDims - 1; ++i) {
        bound[i] = SAFEARRAY::rgsabound[i];
    }
    bound[SAFEARRAY::cDims-1].lLbound = 0;
    bound[SAFEARRAY::cDims-1].cElements = size;

    resize(bound);
    delete[] bound;
}


/** \brief Copy array data.
 */
template <typename T>
auto SafeArray<T>::copy() const
    -> This
{
    This copy;
    if (SafeArrayCopy(this, &(&copy)) == E_OUTOFMEMORY) {
        throw std::runtime_error("E_OUTOFMEMORY from SafeArrayCopy()\n");
    }
    copy.lock();

    return copy;
}


/** \brief Convert array to STL vector.
 *
 *  \warning This will flatten any multidimensional data to a linear
 *  vector.
 */
template <typename T>
SafeArray<T>::operator std::vector<T>() const
{
    return std::vector<T>(begin(), end());
}


/** \brief Write implementation for constexpr.
 */
template <typename T>
constexpr VARTYPE SafeArray<T>::vt;


}   /* autocom */
