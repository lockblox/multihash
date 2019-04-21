#pragma once

#include <multihash/algorithm.h>
#include <varint/varint.h>
#include <cassert>
#include <iomanip>
#include <memory>
#include <vector>

namespace multihash {

/** Provides a view onto a multihash buffer.
 *
 * Ownership of the underlying data is determined by the template parameter */
template <typename Container = std::string>
class multihash {
 public:
  /** Construct a multihash view from data */
  explicit multihash(Container data);

  /** Create a multihash from components */
  template <typename T>
  multihash(typename std::enable_if<
                std::is_same<varint::static_capacity_tag,
                             typename varint::container_traits<
                                 Container>::capacity_type>::value &&
                    std::is_same<T, Container>::value,
                code_type>::type code,
            string_view digest, T data);

  template <typename T>
  multihash(typename std::enable_if<
                std::is_same<varint::dynamic_capacity_tag,
                             typename varint::container_traits<
                                 Container>::capacity_type>::value &&
                    std::is_convertible<T, string_view>::value,
                code_type>::type code,
            T digest);

  /** Assign from another multihash */
  multihash& operator=(const multihash& rhs) = default;

  /** Copy construct from another multihash */
  multihash(const multihash& rhs) = default;

  /** Assign the contents of a buffer to this multihash */
  template <typename Buffer>
  multihash& operator=(Buffer data);

  /** Returns the hash code used to generate the digest */
  code_type code() const;
  /** Returns the digest */
  string_view digest() const;
  /** Returns a pointer to the beginning of the buffer */
  const char* data() const;
  /** Returns the size of the buffer in bytes */
  std::size_t size() const;

  /** Perform element-wise comparison with another multihash */
  template <typename OtherContainer>
  bool operator==(const multihash<OtherContainer>& rhs) const;
  /** Perform element-wise comparison with another multihash */
  bool operator!=(const multihash& rhs) const;
  /** Perform element-wise comparison with another multihash */
  bool operator<(const multihash& rhs) const;
  /** Perform element-wise comparison with another multihash */
  bool operator>(const multihash& rhs) const;

  /** Determine whether the given buffer contains a valid multihash */
  static std::pair<bool, std::string> validate(string_view buffer);

 private:
  Container data_;
};

constexpr std::size_t size(code_type code, string_view digest);
constexpr std::size_t size(code_type code, std::size_t digest_size);

/** Write a multihash header into output */
template <typename OutputIterator>
OutputIterator write(code_type code, std::size_t digest_size,
                     OutputIterator output);

/** Write multihash into output */
template <typename OutputIterator>
OutputIterator write(code_type code, string_view digest, OutputIterator output);

/** IMPLEMENTATION */

template <typename Container>
multihash<Container>::multihash(Container data) : data_(std::move(data)) {
  static_assert(std::is_same<char, typename Container::value_type>::value);
}

template <typename Container>
template <typename T>
multihash<Container>::multihash(
    typename std::enable_if<
        std::is_same<varint::dynamic_capacity_tag,
                     typename varint::container_traits<
                         Container>::capacity_type>::value &&
            std::is_convertible<T, string_view>::value,
        code_type>::type code,
    T digest) {
  assert(digest.size() < 128);
  data_.reserve(digest.size() + 1);
  data_.push_back(code);
  data_.push_back(char(digest.size()));
  std::copy(digest.begin(), digest.end(), std::back_inserter(data_));
}

template <typename Container>
template <typename T>
multihash<Container>::multihash(
    typename std::enable_if<
        std::is_same<varint::static_capacity_tag,
                     typename varint::container_traits<
                         Container>::capacity_type>::value &&
            std::is_same<T, Container>::value,
        code_type>::type code,
    string_view digest, T data)
    : data_(std::move(data)) {
  auto output = string_span(data_);
  write(code, digest, output.begin());
}

template <typename Container>
code_type multihash<Container>::code() const {
  return static_cast<code_type>(data_[0]);
}

template <typename Container>
std::string_view multihash<Container>::digest() const {
  auto view = std::string_view(&data_[2], data_.size() - 2);
  return view;
}

template <typename Container>
const char* multihash<Container>::data() const {
  return data_.data();
}

template <typename Container>
std::size_t multihash<Container>::size() const {
  return data_.size();
}

template <typename Container>
template <typename OtherContainer>
bool multihash<Container>::operator==(
    const multihash<OtherContainer>& rhs) const {
  auto begin = std::begin(data_);
  auto length = std::min(size(), rhs.size());
  auto rhs_view = std::string_view(rhs.data(), rhs.size());
  return std::equal(begin, begin + length, rhs_view.begin());
}

template <typename Container>
bool multihash<Container>::operator!=(const multihash& rhs) const {
  return !(*this == rhs);
}

template <typename Container>
bool multihash<Container>::operator<(const multihash& rhs) const {
  return data_ < rhs.data_;
}

template <typename Container>
bool multihash<Container>::operator>(const multihash& rhs) const {
  return data_ > rhs.data_;
}

template <typename Container>
template <typename Buffer>
multihash<Container>& multihash<Container>::operator=(Buffer data) {
  data_ = Container(data.data(), data.size());
  return *this;
}

template <typename Container>
std::pair<bool, std::string> multihash<Container>::validate(
    string_view buffer) {
  auto result = std::make_pair(true, std::string{});
  if (buffer.size() < 3) {
    result = std::make_pair(false, "Data must be at least 3 bytes");
  } else if (buffer.size() >
             129) {  // maximum digest size of 127 + 2 byte header
    result = std::make_pair(false, "Data must be at most 129 bytes");
  } else if (buffer[0] < 0) {
    result = std::make_pair(false, "Hash codes greater than 127 unsupported");
  } else {
    auto size = uint8_t(buffer[1]);
    auto digest_size = buffer.size() - 3;
    if (digest_size != size) {
      auto msg = std::string{"Header misreports digest size as "} +
                 std::to_string(size) + "; true size is " +
                 std::to_string(digest_size);
      result = std::make_pair(false, msg);
    }
  }
  return result;
}

constexpr std::size_t size(code_type code, string_view digest) {
  assert(digest.size() < 127);
  return sizeof(code) + digest.size() + 1;
}

constexpr std::size_t size(code_type code, std::size_t digest_size) {
  assert(digest_size < 127);
  return sizeof(code) + 1 + digest_size;
}

template <typename OutputIterator>
OutputIterator write(code_type code, std::size_t digest_size,
                     OutputIterator output) {
  assert(code < 128);
  assert(digest_size < 128);
  *output++ = code;
  *output++ = char(digest_size);
  return output;
}

template <typename OutputIterator>
OutputIterator write(code_type code, string_view digest,
                     OutputIterator output) {
  output = write(code, digest.size(), output);
  std::copy(digest.begin(), digest.end(), output);
  return output;
}
}  // namespace multihash