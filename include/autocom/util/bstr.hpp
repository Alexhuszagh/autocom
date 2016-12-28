//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCom
 *  \brief c++ BSTR wrapper.
 */

#pragma once

#include <wtypes.h>

#include <iterator>
#include <string>


namespace autocom
{
// OBJECTS
// -------


/** \brief C++ wrapper around a BSTR.
 *
 *  \warning This class does not efficiently allocate memory for
 *  dynamically-sized objects. Every single operation that changes size
 *  causes a reallocation. Consider yourself warned.
 */
struct Bstr
{
    BSTR string = nullptr;

    // MEMBER TYPES
    // ------------
    typedef wchar_t value_type;
    typedef wchar_t& reference;
    typedef const reference const_reference;
    typedef wchar_t* pointer;
    typedef const wchar_t* const_pointer;
    typedef pointer iterator;
    typedef const_pointer const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    // MEMBER FUNCTIONS
    Bstr() = default;
    Bstr(const Bstr &other);
    Bstr & operator=(const Bstr &other);
    Bstr(Bstr &&other);
    Bstr & operator=(Bstr &&other);
    virtual ~Bstr();

    Bstr(const std::string &string);
    Bstr(const std::wstring &string);
    Bstr(const char *cstring);
    Bstr(const wchar_t *cstring);
    Bstr(const char *array,
        const size_t length);
    Bstr(const wchar_t *array,
        const size_t length);

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

    // CAPACITY
    size_t size() const;
    size_t length() const;
    void clear();
    bool empty() const;

    // ELEMENT ACCESS
    reference operator[](size_t position);
    const_reference operator[](size_t position) const;
    reference at(size_t position);
    const_reference at(size_t position) const;
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;

    // MODIFIERS
    BSTR copy() const;
    void push_back(const wchar_t c);

    template <typename... Ts>
    Bstr & operator +=(Ts&&... ts);

    template <typename... Ts>
    Bstr & append(Ts&&... ts);

    template <typename... Ts>
    Bstr & assign(Ts&&... ts);

    // OPERATORS
    const BSTR & data() const;
    explicit operator bool() const;
    explicit operator BSTR() const;

    // FRIENDS
    friend bool operator==(const Bstr &left,
        const Bstr &right);
    friend bool operator!=(const Bstr &left,
        const Bstr &right);
    friend void swap(Bstr &left,
        Bstr &right);
};


// OPERATOR
// --------


/** \brief Append to string.
 */
template <typename... Ts>
Bstr & Bstr::operator+=(Ts&&... ts)
{
    std::wstring wide(string, size());
    wide.append(AUTOCOM_FWD(ts)...);
    clear();
    string = SysAllocStringLen(wide.data(), wide.size());

    return *this;
}


/** \brief Append to string.
 */
template <typename... Ts>
Bstr & Bstr::append(Ts&&... ts)
{
    return operator+=(AUTOCOM_FWD(ts)...);
}


/** \brief Append string contents.
 */
template <typename... Ts>
Bstr & Bstr::assign(Ts&&... ts)
{
    return operator=(std::move(Bstr(AUTOCOM_FWD(ts)...)));
}


}   /* autocom */
