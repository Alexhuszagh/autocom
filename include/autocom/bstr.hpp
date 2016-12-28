//  :copyright: (c) 2015-2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup AutoCom
 *  \brief c++ BSTR wrapper.
 */

#include <wtypes.h>

#include <iterator>



namespace autocom
{
// OBJECTS
// -------


/** \brief C++ wrapper around a BSTR.
 *
 *  \warning This class does not efficiently allocate memory for
 *  dynamically-sized objects.
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
    typedef const pointer const_pointer;
    typedef pointer iterator;
    typedef const_pointer const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef reverse_iterator const_reverse_iterator;

    // MEMBER FUNCTIONS
    Bstr() = default;
    Bstr(const Bstr &other);
    Bstr & operator=(const Bstr &other);
    Bstr(Bstr &&other);
    Bstr & operator=(Bstr &&other);
    virtual ~Bstr();

    // ITERATORS
    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    reverse_iterator rbegin() noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;

    // TODO: need custom constructors and overloads
    // operator +=
    // append();
    // operator[];
    // at();
    // push_back
    // assign
    // insert
    // erase

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

    // OPERATORS
    const BSTR & data() const;

    // FRIENDS
    friend bool operator==(const Bstr &left,
        const Bstr &right);
    friend bool operator!=(const Bstr &left,
        const Bstr &right);
    friend void swap(Bstr &left,
        Bstr &right);
};


}   /* autocom */
