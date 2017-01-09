//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup AutoCOM
 *  \brief c++ BSTR wrapper.
 */

#include "autocom/bstr.hpp"
#include "autocom/encoding/converters.hpp"

#include <cassert>
#include <cwchar>


namespace autocom
{
// OBJECTS
// -------


/** \brief Copy constructor.
 */
Bstr::Bstr(const Bstr &other):
    string(other.copy())
{}


/** \brief Copy asignment operator.
 */
Bstr & Bstr::operator=(const Bstr &other)
{
    clear();
    string = other.copy();
    return *this;
}


/** \brief Move constructor.
 */
Bstr::Bstr(Bstr &&other)
{
    std::swap(string, other.string);
}


/** \brief Move asignment operator.
 */
Bstr & Bstr::operator=(Bstr &&other)
{
    std::swap(string, other.string);
    return *this;
}


/** \brief Virtual destructor.
 */
Bstr::~Bstr()
{
    clear();
}



/** \brief Copy asignment operator.
 */
Bstr & Bstr::operator=(const BSTR &other)
{
    clear();
    string = SysAllocStringLen(other, SysStringLen(other));
    return *this;
}


/** \brief Move asignment operator.
 */
Bstr & Bstr::operator=(BSTR &&other)
{
    clear();
    string = std::move(other);
    return *this;
}


/** \brief Initialize string from narrow string.
 */
Bstr::Bstr(const std::string &string)
{
    auto wide = WIDE(string);
    this->string = SysAllocStringLen(wide.data(), wide.size());
}


/** \brief Initialize string from wide string.
 */
Bstr::Bstr(const std::wstring &string):
    string(SysAllocStringLen(string.data(), string.size()))
{}


/** \brief Initialize string from variant.
 */
Bstr::Bstr(VARIANT &variant)
{
    reset(variant);
}



/** \brief Initialize string from narrow C-string.
 */
Bstr::Bstr(const char *cstring)
{
    auto wide = WIDE(std::string(cstring));
    this->string = SysAllocStringLen(wide.data(), wide.size());
}


/** \brief Initialize string from wide C-string.
 */
Bstr::Bstr(const wchar_t *cstring)
{
    this->string = SysAllocStringLen(cstring, wcslen(cstring));
}


/** \brief Initialize string from narrow character array.
 */
Bstr::Bstr(const char *array,
    const size_t length)
{
    auto wide = WIDE(std::string(array, length));
    this->string = SysAllocStringLen(wide.data(), wide.size());
}


/** \brief Initialize string from wide character array.
 */
Bstr::Bstr(const wchar_t *array,
        const size_t length)
{
    this->string = SysAllocStringLen(array, length);
}


/** \brief Get iterator at beginning of string.
 */
auto Bstr::begin() noexcept
    -> iterator
{
    return string ? &string[0] : nullptr;
}


/** \brief Get iterator past end of string.
 */
auto Bstr::end() noexcept
    -> iterator
{
    return begin() + size();
}


/** \brief Get iterator at beginning of string.
 */
auto Bstr::begin() const noexcept
    -> const_iterator
{
    return cbegin();
}


/** \brief Get iterator past end of string.
 */
auto Bstr::end() const noexcept
    -> const_iterator
{
    return cend();
}


/** \brief Get iterator at beginning of string.
 */
auto Bstr::cbegin() const noexcept
    -> const_iterator
{
    return string ? &string[0] : nullptr;
}


/** \brief Get iterator past end of string.
 */
auto Bstr::cend() const noexcept
    -> const_iterator
{
    return cbegin() + size();
}


/** \brief Get iterator at reverse beginning of string.
 */
auto Bstr::rbegin() noexcept
    -> reverse_iterator
{
    return reverse_iterator(end());
}


/** \brief Get iterator past reverse end of string.
 */
auto Bstr::rend() noexcept
    -> reverse_iterator
{
    return reverse_iterator(begin());
}


/** \brief Get iterator at reverse beginning of string.
 */
auto Bstr::rbegin() const noexcept
    -> const_reverse_iterator
{
    return const_reverse_iterator(cend());
}


/** \brief Get iterator past reverse end of string.
 */
auto Bstr::rend() const noexcept
    -> const_reverse_iterator
{
    return const_reverse_iterator(cbegin());
}


/** \brief Get iterator at reverse beginning of string.
 */
auto Bstr::crbegin() const noexcept
    -> const_reverse_iterator
{
    return const_reverse_iterator(cend());
}


/** \brief Get iterator past reverse end of string.
 */
auto Bstr::crend() const noexcept
    -> const_reverse_iterator
{
    return const_reverse_iterator(cbegin());
}


/** \brief Get length of BSTR.
 */
size_t Bstr::size() const
{
    if (string) {
        return SysStringLen(string);
    }

    return 0;
}


/** \brief Get length of BSTR.
 */
size_t Bstr::length() const
{
    return size();
}


/** \brief Clear BSTR.
 */
void Bstr::clear()
{
    if (string) {
        SysFreeString(string);
        string = nullptr;
    }
}


/** \brief Check if BSTR is empty.
 */
bool Bstr::empty() const
{
    if (string) {
        return SysStringLen(string) == 0;
    }

    return true;
}


/** \brief Access character at index.
 */
auto Bstr::at(size_t position)
    -> reference
{
    if (position > size()) {
        throw std::out_of_range("Index is out of range");
    }

    return string[position];
}


/** \brief Access character at index.
 */
auto Bstr::at(size_t position) const
    -> const_reference
{
    if (position > size()) {
        throw std::out_of_range("Index is out of range");
    }

    return string[position];
}


/** \brief Get reference to first element in string.
 */
auto Bstr::front()
    -> reference
{
    assert(!empty() && "bstr::front(): string is empty");
    return *string;
}


/** \brief Get reference to first element in string.
 */
auto Bstr::front() const
    -> const_reference
{
    assert(!empty() && "bstr::front(): string is empty");
    return *string;
}


/** \brief Get reference to last element in string.
 */
auto Bstr::back()
    -> reference
{
    assert(!empty() && "bstr::back(): string is empty");
    return *(string + size() - 1);
}


/** \brief Get reference to last element in string.
 */
auto Bstr::back() const
    -> const_reference
{
    assert(!empty() && "bstr::back(): string is empty");
    return *(string + size() - 1);
}


/** \brief Copy BSTR.
 */
BSTR Bstr::copy() const
{
    if (string) {
        return SysAllocStringLen(string, SysStringLen(string));
    }

    return nullptr;
}


/** \brief Append character to string.
 */
void Bstr::push_back(const wchar_t c)
{
    std::wstring wide(string, size());
    wide.push_back(c);
    clear();
    string = SysAllocStringLen(wide.data(), wide.size());
}


/** \brief Release handle to string.
 */
void Bstr::reset()
{
    clear();
}


/** \brief Reset BSTR and own new string.
 */
void Bstr::reset(BSTR bstr)
{
    clear();
    string = bstr;
}


/** \brief Reset BSTR and own new string from VARIANT.
 */
void Bstr::reset(VARIANT &variant)
{
    clear();
    if (variant.vt & (VT_BSTR | VT_BYREF)) {
        throw std::runtime_error("Cannot take ownership of value by reference");
    } else if (variant.vt & VT_BSTR) {
        string = variant.bstrVal;
        variant.bstrVal = nullptr;
        variant.vt = VT_EMPTY;
    }
}


/** \brief Get BSTR data.
 */
BSTR & Bstr::data()
{
    return string;
}


/** \brief Get BSTR data.
 */
const BSTR & Bstr::data() const
{
    return string;
}


/** \brief Check if string is not empty.
 */
Bstr::operator bool() const
{
    return !empty();
}


/** \brief Convert type to BSTR.
 */
Bstr::operator BSTR()
{
    return string;
}


/** \brief Convert type to LPCOLESTR.
 */
Bstr::operator LPCOLESTR() const
{
    return string;
}


/** \brief Convert type explicitly to narrow string.
 */
Bstr::operator std::string() const
{
    return NARROW(std::wstring(string, size()));
}


/** \brief Convert type explicitly to wide string.
 */
Bstr::operator std::wstring() const
{
    return std::wstring(string, size());
}


/** \brief Equality operator.
 */
bool operator==(const Bstr &left,
    const Bstr &right)
{
    if (left.size() == right.size()) {
        auto l = left.cbegin();
        auto r = right.cbegin();
        while (l != left.cend()) {
            if (*l++ != *r++) {
                return false;
            }
        }
        return true;
    }

    return false;
}


/** \brief Inequality operator.
 */
bool operator!=(const Bstr &left,
    const Bstr &right)
{
    return !(left == right);
}


/** \brief Swap left and right strings.
 */
void swap(Bstr &left,
    Bstr &right)
{
    std::swap(left.string, right.string);
}


}   /* autocom */
