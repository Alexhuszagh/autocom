//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCom
 *  \brief c++ BSTR wrapper.
 */

#include "autocom.hpp"

#include <cassert>


namespace autocom
{
// OBJECTS
// -------


/** \brief Copy constructor.
 */
Bstr::Bstr(const Bstr &other):
    string(other.copy())
{}

//SysAllocStringLen
// SysAllocStringByteLen


/** \brief Copy asignment operator.
 */
Bstr & Bstr::operator=(const Bstr &other)
{
    string = other.copy();
    return *this;
}


/** \brief Move constructor.
 */
Bstr::Bstr(Bstr &&other):
    string(std::move(other.string))
{
    other.string = nullptr;
}


/** \brief Move asignment operator.
 */
Bstr & Bstr::operator=(Bstr &&other)
{
    string = std::move(other.string);
    other.string = nullptr;
    return *this;
}


/** \brief Virtual destructor.
 */
Bstr::~Bstr()
{
    clear();
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
auto Bstr::operator[](size_t position)
    -> reference
{
    return at(position);
}


/** \brief Access character at index.
 */
auto Bstr::operator[](size_t position) const
    -> const_reference
{
    return at(position);
}


/** \brief Access character at index.
 */
auto Bstr::at(size_t position)
    -> reference
{
    if (position > size()) {
        throw std::out_of_range("Index is out of range");
    }

    return at(position);
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
    return *begin();
}


/** \brief Get reference to first element in string.
 */
auto Bstr::front() const
    -> const_reference
{
    assert(!empty() && "bstr::front(): string is empty");
    return *cbegin();
}


/** \brief Get reference to last element in string.
 */
auto Bstr::back()
    -> reference
{
    assert(!empty() && "bstr::back(): string is empty");
    return *(end()-1);
}


/** \brief Get reference to last element in string.
 */
auto Bstr::back() const
    -> const_reference
{
    assert(!empty() && "bstr::back(): string is empty");
    return *(cend()-1);
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


/** \brief Get BSTR data.
 */
const BSTR & Bstr::data() const
{
    return string;
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
