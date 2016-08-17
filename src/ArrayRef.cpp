#include <cstring>
#include <multihash/ArrayRef.h>

namespace multihash
{

ArrayRef::ArrayRef() : data_(nullptr), size_(0)
{
}

ArrayRef::ArrayRef(const char* data, size_t size) : data_(data), size_(size)
{
}

ArrayRef::ArrayRef(const std::string& data)
    : data_(data.data()), size_(data.size())
{
}

ArrayRef::ArrayRef(const std::vector<char>& data)
    : data_(data.data()), size_(data.size())
{
}

ArrayRef::iterator ArrayRef::begin()
{
    return const_cast<char*>(data_);
}

ArrayRef::iterator ArrayRef::end()
{
    return const_cast<char*>(&data_[size_]);
}

ArrayRef::const_iterator ArrayRef::cbegin() const
{
    return data_;
}

ArrayRef::const_iterator ArrayRef::cend() const
{
    return &data_[size_];
}

ArrayRef::const_iterator ArrayRef::begin() const
{
    return cbegin();
}

ArrayRef::const_iterator ArrayRef::end() const
{
    return cend();
}

ArrayRef::operator std::string() const
{
    return std::string(data(), size());
}

ArrayRef::operator std::vector<char>() const
{
    return std::vector<char>(cbegin(), cend());
}

const char* ArrayRef::data() const
{
    return data_;
}

char* ArrayRef::data()
{
    return const_cast<char*>(data_);
}

size_t ArrayRef::size() const
{
    return size_;
}

size_t ArrayRef::length() const
{
    return size_;
}

bool ArrayRef::empty() const
{
    return 0 == size_;
}

const char& ArrayRef::operator[](size_t n) const
{
    return data_[n];
}

char& ArrayRef::operator[](size_t n)
{
    return const_cast<char&>(data_[n]);
}

const char& ArrayRef::at(size_t n) const
{
    if (n >= size_)
    {
        throw std::out_of_range("Index out of range");
    }
    return data_[n];
}

char& ArrayRef::at(size_t n)
{
    if (n >= size_)
    {
        throw std::out_of_range("Index out of range");
    }
    return const_cast<char&>(data_[n]);
}

void ArrayRef::clear()
{
    data_ = nullptr;
    size_ = 0;
}

bool operator==(const ArrayRef lhs, const ArrayRef rhs)
{
    return (lhs.size() == rhs.size() &&
            0 == std::memcmp(lhs.data(), rhs.data(), lhs.size()));
}

bool operator!=(const ArrayRef lhs, const ArrayRef rhs)
{
    return !(lhs == rhs);
}

} // namespace multihash
