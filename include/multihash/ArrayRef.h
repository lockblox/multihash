#pragma once
#include <stdexcept>
#include <string>
#include <vector>

namespace multihash
{

/**@brief ArrayRef is a non-owning view onto a range of memory */
class ArrayRef
{
public:
    using iterator = char*;
    using const_iterator = const char*;

    /**@brief Create a view on an empty array */
    ArrayRef();

    /**@brief Create a view on an array with elements data[0]-data[size-1] */
    ArrayRef(const char* data, size_t size);

    /**@brief Create a view onto string contents */
    ArrayRef(const std::string& data);

    /**@brief Create a view onto a vector of chars */
    ArrayRef(const std::vector<char>& data);

    /**@brief Get a read/write iterator pointing to first element */
    iterator begin();

    /**@brief Get a read/write iterator pointing to one past last element */
    iterator end();

    /**@brief Get a read-only iterator pointing to first element */
    const_iterator cbegin() const;

    /**@brief Get a read-only iterator pointing to one past last element */
    const_iterator cend() const;

    /**@brief Get a read-only iterator pointing to first element */
    const_iterator begin() const;

    /**@brief Get a read-only iterator pointing to one past last element */
    const_iterator end() const;

    /**@brief Convert to a string */
    explicit operator std::string() const;

    /**@brief Convert to a vector */
    explicit operator std::vector<char>() const;

    /**@brief Get a pointer to the first element of the array */
    const char* data() const;

    char* data();

    /**@brief Get the array length in bytes */
    size_t size() const;

    /**@copydoc size */
    size_t length() const;

    /**@brief Return true iff the array length is 0 */
    bool empty() const;

    /**@brief Get un-checked access to the nth array element */
    const char& operator[](size_t n) const;

    char& operator[](size_t n);

    /**@brief Get the nth array element with bounds checking*/
    const char& at(size_t n) const;

    char& at(size_t n);

    /**@brief Reset the view to an empty state */
    void clear();

private:
    const char* data_;
    size_t size_;
};

bool operator==(const ArrayRef lhs, const ArrayRef rhs);

bool operator!=(const ArrayRef lhs, const ArrayRef rhs);

} // namespace multihash
