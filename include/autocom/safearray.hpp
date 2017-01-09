//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief COM SafeArray wrapper.
 */

#pragma once

#include "util/exception.hpp"
#include "util/type.hpp"

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
    // WinAPI
    // ULONG cElements;
    // LONG  lLbound;

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

    // ASSIGNERS
    void assign(SAFEARRAY &safearray);
    void assign(VARIANT &variant);

public:
    // WinAPI
    // USHORT         cDims;
    // USHORT         fFeatures;
    // ULONG          cbElements;
    // ULONG          cLocks;
    // PVOID          pvData;
    // SAFEARRAYBOUND rgsabound[1];

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
    SafeArray(const std::vector<T> &other);
    SafeArray(const std::initializer_list<T> other);
    SafeArray(VARIANT &variant);
    template <typename Iter>
    SafeArray(Iter begin,
        Iter end);

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
    template <typename U>
    typename std::enable_if<std::is_pointer<U>::value, reference>::type
    operator[](U indices);

    template <typename U>
    typename std::enable_if<std::is_pointer<U>::value, const_reference>::type
    operator[](U indices) const;

    template <typename U>
    typename std::enable_if<std::is_pointer<U>::value, reference>::type
    at(U indices);

    template <typename U>
    typename std::enable_if<std::is_pointer<U>::value, const_reference>::type
    at(U indices) const;

    reference operator[](const LONG index);
    const_reference operator[](const LONG index) const;
    reference at(const LONG index);
    const_reference at(const LONG index) const;

    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;
    const_pointer data() const;

    // MODIFIERS
    void resize(SafeArrayBound *bound);
    void resize(const LONG size);
    void reset();
    void reset(SAFEARRAY &safearray);
    void reset(VARIANT &variant);

    // DATA
    This copy() const;
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
    auto hr = SafeArrayLock(this);
    if (FAILED(hr)) {
        printf("FUCK: %d\n", hr);
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
    assign(*array);
}


/** \brief Destroy array.
 */
template <typename T>
void SafeArray<T>::close()
{
    unlock();
    SafeArrayDestroy(this);
}


/** \brief Assign data from SAFEARRAY.
 */
template <typename T>
void SafeArray<T>::assign(SAFEARRAY &safearray)
{
    reinterpret_cast<SAFEARRAY&>(*this) = safearray;
}


/** \brief Assign data from VARIANT.
 */
template <typename T>
void SafeArray<T>::assign(VARIANT &variant)
{
    if (variant.vt & (VT_BYREF)) {
        throw std::runtime_error("Cannot take ownership of value by reference");
    } else if (variant.vt & VT_ARRAY) {
        assign(*variant.parray);
        variant.parray = nullptr;
        variant.vt = VT_EMPTY;
    } else {
        throw ComTypeError("VT_ARRAY", std::to_string(variant.vt), "&");
    }
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
    assign(*array);
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
    assign(*array);
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


/** \brief Initialize SafeArray from vector.
 */
template <typename T>
SafeArray<T>::SafeArray(const std::vector<T> &other)
{
    SafeArrayBound bound(other.size());
    create(1, &bound);
    lock();

    auto *array = reinterpret_cast<pointer>(SAFEARRAY::pvData);
    for (const auto &item: other) {
        *array++ = item;
    }
}


/** \brief Initialize SafeArray from initializer list.
 */
template <typename T>
SafeArray<T>::SafeArray(const std::initializer_list<T> other)
{
    SafeArrayBound bound(other.size());
    create(1, &bound);
    lock();

    auto *array = reinterpret_cast<pointer>(SAFEARRAY::pvData);
    for (const auto &item: other) {
        *array++ = item;
    }
}


/** \brief Initialize SafeArray from variant.
 */
template <typename T>
SafeArray<T>::SafeArray(VARIANT &variant)
{
    assign(variant);
    lock();
}


/** \brief Range constructor.
 */
template <typename T>
template <typename Iter>
SafeArray<T>::SafeArray(Iter begin,
    Iter end)
{
    static_assert(std::is_same<typename Iter::value_type, T>::value, "Value type of iterator must be same as array.");

    SafeArrayBound bound(end - begin);
    create(1, &bound);
    lock();

    auto *array = reinterpret_cast<pointer>(SAFEARRAY::pvData);
    while (begin < end) {
        *array++ = *begin++;
    }
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
    return reinterpret_cast<iterator>(SAFEARRAY::pvData);
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
    return const_cast<const_iterator>(begin());
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
    if (size >= SAFEARRAY::cDims) {
        throw std::out_of_range("SafeArray:: Size requested is out of bounds");
    }

    if (size < 0) {
        // get all dimensions
        size_t size = 1;
        for (USHORT i = 0; i < SAFEARRAY::cDims; ++i) {
            const auto &bound = SAFEARRAY::rgsabound[i];
            size *= (bound.cElements - bound.lLbound);
        }
        return size;
    } else {
        // get single dimension
        const auto &bound = SAFEARRAY::rgsabound[size];
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


/** \brief Get element at multi-dimensional index.
 */
template <typename T>
template <typename U>
typename std::enable_if<std::is_pointer<U>::value, T&>::type
SafeArray<T>::operator[](U indices)
{
    return at(indices);
}


/** \brief Get element at multi-dimensional index.
 */
template <typename T>
template <typename U>
typename std::enable_if<std::is_pointer<U>::value, const T&>::type
SafeArray<T>::operator[](U indices) const
{
    return at(indices);
}


/** \brief Get element at multi-dimensional index.
 */
template <typename T>
template <typename U>
typename std::enable_if<std::is_pointer<U>::value, T&>::type
SafeArray<T>::at(U indices)
{
    T *data;
    SafeArrayPtrOfIndex(this, indices, (void**) &data);

    return *data;
}


/** \brief Get element at multi-dimensional index.
 */
template <typename T>
template <typename U>
typename std::enable_if<std::is_pointer<U>::value, const T&>::type
SafeArray<T>::at(U indices) const
{
    T *data;
    SafeArrayPtrOfIndex(this, indices, (void**) &data);

    return const_cast<const_reference>(data);
}


/** \brief Get element at index (STL-like).
 */
template <typename T>
auto SafeArray<T>::operator[](const LONG index)
    -> reference
{
    return at(index);
}


/** \brief Get element at index (STL-like).
 */
template <typename T>
auto SafeArray<T>::operator[](const LONG index) const
    -> const_reference
{
    return at(index);
}


/** \brief Get element at index (STL-like).
 */
template <typename T>
auto SafeArray<T>::at(const LONG index)
    -> reference
{
    auto *array = reinterpret_cast<pointer>(SAFEARRAY::pvData);
    return array[index];
}


/** \brief Get element at index (STL-like).
 */
template <typename T>
auto SafeArray<T>::at(const LONG index) const
    -> const_reference
{
    auto *array = reinterpret_cast<pointer>(SAFEARRAY::pvData);
    return const_cast<const_reference>(array[index]);
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


/** \brief Reset and clear current array.
 */
template <typename T>
void SafeArray<T>::reset()
{
    close();
    SafeArrayBound bound(0);
    create(1, &bound);
    lock();
}


/** \brief Reset SAFEARRAY and own new array.
 */
template <typename T>
void SafeArray<T>::reset(SAFEARRAY &safearray)
{
    close();
    assign(safearray);
    lock();
}


/** \brief Change dimension bounds size of least significant bound.
 */
template <typename T>
void SafeArray<T>::reset(VARIANT &variant)
{
    close();
    assign(variant);
    lock();
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


/** \brief Write implementation for constexpr.
 */
template <typename T>
constexpr VARTYPE SafeArray<T>::vt;


}   /* autocom */
