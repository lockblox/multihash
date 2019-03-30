#pragma once

#include <multihash/algorithm.h>
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
  /** Returns the size of the digest in bytes */
  std::size_t digest_size() const;

  /** Perform element-wise comparison with another multihash */
  template <typename OtherContainer>
  bool operator==(const multihash<OtherContainer>& rhs) const;
  /** Perform element-wise comparison with another multihash */
  bool operator!=(const multihash& rhs) const;
  /** Perform element-wise comparison with another multihash */
  bool operator<(const multihash& rhs) const;
  /** Perform element-wise comparison with another multihash */
  bool operator>(const multihash& rhs) const;

  /** Create a multihash from components */
  static multihash create(string_span output, code_type code,
                          string_view digest);

  /** Write a multihash header into output */
  static std::size_t write(string_span output, code_type code,
                           std::size_t digest_size);

  /** Write multihash into output */
  static std::size_t write(string_span output, code_type code,
                           string_view digest_size);

  static constexpr std::size_t size(code_type code, string_view digest);
  static constexpr std::size_t size(code_type code, std::size_t digest);

  /** Determine whether the given buffer contains a valid multihash */
  static std::pair<bool, std::string> validate(string_view buffer);

 private:
  Container data_;
};

/** IMPLEMENTATION */

template <typename Container>
multihash<Container>::multihash(Container data) : data_(std::move(data)) {
  static_assert(std::is_same<char, typename Container::value_type>::value);
}

template <typename Container>
code_type multihash<Container>::code() const {
  return static_cast<code_type>(data_[0]);
}

template <typename Container>
std::string_view multihash<Container>::digest() const {
  auto view = std::string_view(&data()[2], data().size() - 2);
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
std::size_t multihash<Container>::digest_size() const {
  return data().size() - 3;
}

template <typename Container>
template <typename Buffer>
multihash<Container>& multihash<Container>::operator=(Buffer data) {
  data_ = Container(data.data(), data.size());
  return *this;
}

template <typename Container>
std::size_t multihash<Container>::write(string_span output, code_type code,
                                        std::size_t digest_size) {
  assert(std::size_t(output.size()) >= size(code, digest_size));
  auto length = sizeof(code) + 1;
  auto it = output.begin();
  std::copy(&code, &code + sizeof(code), it);
  it += sizeof(code);
  *it = char(digest_size);
  return length;
}

template <typename Container>
std::size_t multihash<Container>::write(string_span output, code_type code,
                                        string_view digest) {
  assert(std::size_t(output.size()) >= size(code, digest));
  auto size = write(output, code, digest.size());
  auto it = output.begin() + size;
  std::copy(digest.begin(), digest.end(), it);
  size += digest.size();
  return size;
}

template <typename Container>
multihash<Container> multihash<Container>::create(string_span output,
                                                  code_type code,
                                                  string_view digest) {
  write(output, code, digest);
  auto data = Container(output.data(), output.size());
  return multihash(data);
}

template <typename Container>
constexpr std::size_t multihash<Container>::size(code_type code,
                                                 string_view digest) {
  assert(digest.size() < 127);
  return sizeof(code) + digest.size() + 1;
}

template <typename Container>
constexpr std::size_t multihash<Container>::size(code_type code,
                                                 std::size_t digest_size) {
  assert(digest_size < 127);
  return sizeof(code) + 1 + digest_size;
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

}  // namespace multihash
