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
  multihash(
      typename std::enable_if<std::is_same<varint::detail::static_extent_t,
                                           typename varint::detail::extent_type<
                                               Container>::type>::value &&
                                  std::is_same<T, Container>::value,
                              varint_view>::type code,
      std::string_view digest, T data);

  template <typename T>
  multihash(
      typename std::enable_if<
          std::is_same<
              varint::detail::dynamic_extent_t,
              typename varint::detail::extent_type<Container>::type>::value &&
              std::is_convertible<T, std::string_view>::value,
          varint_view>::type code,
      T digest);

  /** Assign from another multihash */
  multihash& operator=(const multihash& rhs) = default;

  /** Copy construct from another multihash */
  multihash(const multihash& rhs) = default;

  /** Assign the contents of a buffer to this multihash */
  template <typename Buffer>
  multihash& operator=(Buffer data);

  /** Returns the hash code used to generate the digest */
  varint_view code() const;
  /** Returns the digest */
  std::string_view digest() const;
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

 private:
  /** Re-calculate offsets for code and size */
  void reset_view(varint_view code, std::size_t digest_size);

  Container data_;          /**< Raw buffer data */
  varint_view code_;        /**< Multihash code identifying algorithm */
  varint_view size_;        /**< Size of digest */
  std::string_view digest_; /**< Digest */
  std::string_view view_;   /**< View onto entire buffer */
};

/** Compute the minimum required size to encode a code and digest */
inline std::size_t size(varint_view code, std::string_view digest);

/** Compute the minimum required size to encode a code and digest */
inline std::size_t size(varint_view code, std::size_t digest_size);

/** Write a multihash header into output */
template <typename OutputIterator>
OutputIterator write(varint_view code, std::size_t digest_size,
                     OutputIterator output);

/** Write multihash into output */
template <typename OutputIterator>
OutputIterator write(varint_view code, std::string_view digest,
                     OutputIterator output);

/******************************************************************************/
/** IMPLEMENTATION */
/******************************************************************************/

template <typename Container>
multihash<Container>::multihash(Container data)
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
multihash<Container>::multihash(
    typename std::enable_if<std::is_same<varint::detail::dynamic_extent_t,
                                         typename varint::detail::extent_type<
                                             Container>::type>::value &&
                                std::is_convertible<T, std::string_view>::value,
                            varint_view>::type code,
    T digest) {
  write(code, digest, std::back_inserter(data_));
  reset_view(code, digest.size());
}

template <typename Container>
template <typename T>
multihash<Container>::multihash(
    typename std::enable_if<std::is_same<varint::detail::static_extent_t,
                                         typename varint::detail::extent_type<
                                             Container>::type>::value &&
                                std::is_same<T, Container>::value,
                            varint_view>::type code,
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
varint_view multihash<Container>::code() const {
  return code_;
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
void multihash<Container>::reset_view(varint_view code,
                                      std::size_t digest_size) {
  auto code_view = static_cast<std::string_view>(code);
  code_view = std::string_view(data_.data(), code_view.size());
  auto size_view = std::string_view(code_view.data(),
                                    varint::codecs::uleb128::size(digest_size));
  code_ = varint_view(code_view);
  size_ = varint_view(size_view);
  digest_ = std::string_view(size_view.data(), digest_size);
  view_ = std::string_view(data_.data(),
                           code_view.size() + size_view.size() + digest_size);
}

std::size_t size(varint_view code, std::string_view digest) {
  auto code_view = static_cast<std::string_view>(code);
  assert(digest.size() < 127);
  return code_view.size() + digest.size() + 1;
}

std::size_t size(varint_view code, std::size_t digest_size) {
  auto code_view = static_cast<std::string_view>(code);
  assert(digest_size < 127);
  return code_view.size() + 1 + digest_size;
}

template <typename OutputIterator>
OutputIterator write(varint_view code, std::size_t digest_size,
                     OutputIterator output) {
  auto code_view = static_cast<std::string_view>(code);
  auto size = varint_type<std::string>(digest_size);
  auto size_view = static_cast<std::string_view>(size);
  output = std::copy(code_view.begin(), code_view.end(), output);
  output = std::copy(size_view.begin(), size_view.end(), output);
  return output;
}

template <typename OutputIterator>
OutputIterator write(varint_view code, std::string_view digest,
                     OutputIterator output) {
  output = write(code, digest.size(), output);
  output = std::copy(digest.begin(), digest.end(), output);
  return output;
}

}  // namespace multihash