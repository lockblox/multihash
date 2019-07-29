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
class digest {
 public:
  /** Constructs an empty digest */
  digest() = default;

  /** Constructs a digest from a data buffer */
  explicit digest(Container data);

  /** Creates a multihash from components */
  template <typename T>
  digest(
      typename std::enable_if<std::is_same<varint::detail::static_extent_t,
                                           typename varint::detail::extent_type<
                                               Container>::type>::value &&
                                  std::is_same<T, Container>::value,
                              code_type>::type code,
      std::string_view digest, T data);

  /** Creates a multihash from components */
  template <typename T>
  digest(
      typename std::enable_if<
          std::is_same<
              varint::detail::dynamic_extent_t,
              typename varint::detail::extent_type<Container>::type>::value &&
              std::is_convertible<T, std::string_view>::value,
          code_type>::type code,
      T digest);

  /** Assigns from another multihash */
  digest& operator=(const digest& rhs) = default;

  /** Creates a digest from a copy */
  digest(const digest& rhs) = default;

  /** Assigns the contents of a buffer to this digest */
  template <typename Buffer>
  digest& operator=(Buffer data);

  /** Returns a std::string_view encapsulating the entire multihash buffer */
  operator std::string_view() const;

  /** Checks if the digest is empty */
  constexpr bool empty() const;
  /** Returns the hash code used to generate the digest */
  code_type code() const;
  /** Returns the digest */
  std::string_view value() const;
  /** Returns a pointer to the beginning of the buffer */
  const char* data() const;
  /** Returns the size of the buffer in bytes */
  std::size_t size() const;

  /** Performs element-wise comparison with another multihash */
  template <typename OtherContainer>
  bool operator==(const digest<OtherContainer>& rhs) const;
  /** Performs element-wise comparison with another multihash */
  bool operator!=(const digest& rhs) const;
  /** Performs element-wise comparison with another multihash */
  bool operator<(const digest& rhs) const;
  /** Performs element-wise comparison with another multihash */
  bool operator>(const digest& rhs) const;

 private:
  /** Re-calculates offsets for code and size */
  void reset_view(code_type code, std::size_t digest_size);

  Container data_;          /**< Raw buffer data */
  code_type code_;          /**< Multihash code identifying algorithm */
  code_type size_;          /**< Size of digest */
  std::string_view digest_; /**< Digest */
  std::string_view view_;   /**< View onto entire buffer */
};

/** Computes the minimum required size to encode a code and digest */
inline std::size_t size(code_type code, std::string_view digest);

/** Computes the minimum required size to encode a code and digest */
inline std::size_t size(code_type code, std::size_t digest_size);

/** Writes a multihash header into output */
template <typename OutputIterator>
OutputIterator write(code_type code, std::size_t digest_size,
                     OutputIterator output);

/** Writes multihash into output */
template <typename OutputIterator>
OutputIterator write(code_type code, std::string_view digest,
                     OutputIterator output);

/******************************************************************************/
/** IMPLEMENTATION */
/******************************************************************************/

template <typename Container>
digest<Container>::digest(Container data)
    : data_(std::move(data)),
      code_(std::string_view(data_.data(), data_.size())),
      size_(std::string_view(
          static_cast<std::string_view>(code_).data(),
          data_.size() - static_cast<std::string_view>(code_).size())),
      digest_(std::string_view(
          static_cast<std::string_view>(size_).data(),
          data_.size() - static_cast<std::string_view>(code_).size() -
              static_cast<std::string_view>(size_).size())),
      view_(std::string_view(
          data_.data(), static_cast<std::string_view>(code_).size() +
                            static_cast<std::string_view>(size_).size() +
                            static_cast<std::string_view>(digest_).size())) {
  static_assert(std::is_same<char, typename Container::value_type>::value);
}  // namespace multihash

template <typename Container>
template <typename T>
digest<Container>::digest(
    typename std::enable_if<std::is_same<varint::detail::dynamic_extent_t,
                                         typename varint::detail::extent_type<
                                             Container>::type>::value &&
                                std::is_convertible<T, std::string_view>::value,
                            code_type>::type code,
    T digest) {
  write(code, digest, std::back_inserter(data_));
  reset_view(code, digest.size());
}

template <typename Container>
template <typename T>
digest<Container>::digest(
    typename std::enable_if<std::is_same<varint::detail::static_extent_t,
                                         typename varint::detail::extent_type<
                                             Container>::type>::value &&
                                std::is_same<T, Container>::value,
                            code_type>::type code,
    std::string_view digest, T data)
    : data_(std::move(data)) {
  assert(varint::codecs::uleb128::size(digest.size()) +
             static_cast<std::string_view>(code).size() + digest.size() <=
         static_cast<std::size_t>(data_.size()));
  auto output = string_span(data_);
  write(code, digest, output.begin());
  reset_view(code, digest.size());
}

template <typename Container>
code_type digest<Container>::code() const {
  return code_;
}

template <typename Container>
std::string_view digest<Container>::value() const {
  auto view = std::string_view(&data_[2], data_.size() - 2);
  return view;
}

template <typename Container>
const char* digest<Container>::data() const {
  return data_.data();
}

template <typename Container>
std::size_t digest<Container>::size() const {
  return data_.size();
}

template <typename Container>
template <typename OtherContainer>
bool digest<Container>::operator==(const digest<OtherContainer>& rhs) const {
  auto begin = std::begin(data_);
  auto length = std::min(size(), rhs.size());
  auto rhs_view = std::string_view(rhs.data(), rhs.size());
  return std::equal(begin, begin + length, rhs_view.begin());
}

template <typename Container>
bool digest<Container>::operator!=(const digest& rhs) const {
  return !(*this == rhs);
}

template <typename Container>
bool digest<Container>::operator<(const digest& rhs) const {
  return data_ < rhs.data_;
}

template <typename Container>
bool digest<Container>::operator>(const digest& rhs) const {
  return data_ > rhs.data_;
}

template <typename Container>
template <typename Buffer>
digest<Container>& digest<Container>::operator=(Buffer data) {
  data_ = Container(data.data(), data.size());
  return *this;
}

template <typename Container>
void digest<Container>::reset_view(code_type code, std::size_t digest_size) {
  auto code_view = static_cast<std::string_view>(code);
  code_view = std::string_view(data_.data(), code_view.size());
  auto size_view = std::string_view(code_view.data(),
                                    varint::codecs::uleb128::size(digest_size));
  code_ = code_type(code_view);
  size_ = code_type(size_view);
  digest_ = std::string_view(size_view.data(), digest_size);
  view_ = std::string_view(data_.data(),
                           code_view.size() + size_view.size() + digest_size);
}

template <typename Container>
constexpr bool digest<Container>::empty() const {
  return data_.empty();
}

template <typename Container>
digest<Container>::operator std::string_view() const {
  return std::string_view(data(), size());
}

std::size_t size(code_type code, std::string_view digest) {
  auto code_view = static_cast<std::string_view>(code);
  assert(digest.size() < 127);
  return code_view.size() + digest.size() + 1;
}

std::size_t size(code_type code, std::size_t digest_size) {
  auto code_view = static_cast<std::string_view>(code);
  assert(digest_size < 127);
  return code_view.size() + 1 + digest_size;
}

template <typename OutputIterator>
OutputIterator write(code_type code, std::size_t digest_size,
                     OutputIterator output) {
  auto code_view = static_cast<std::string_view>(code);
  auto size = varint_type<std::string>(digest_size);
  auto size_view = static_cast<std::string_view>(size);
  output = std::copy(code_view.begin(), code_view.end(), output);
  output = std::copy(size_view.begin(), size_view.end(), output);
  return output;
}

template <typename OutputIterator>
OutputIterator write(code_type code, std::string_view digest,
                     OutputIterator output) {
  output = write(code, digest.size(), output);
  output = std::copy(digest.begin(), digest.end(), output);
  return output;
}

}  // namespace multihash