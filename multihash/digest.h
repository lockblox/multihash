#pragma once

#include <multihash/algorithm.h>
#include <istream>
#include <vector>

namespace multihash {

class digest {
 public:
  /** Create a hash by looking up an algorithm in the registry */
  explicit digest(std::unique_ptr<algorithm> algorithm);

  virtual ~digest() = default;

  /** Compute the hash digest of an input range and write to the given output */
  template <typename InputIterator, typename OutputIterator>
  OutputIterator operator()(InputIterator first, InputIterator last,
                            OutputIterator output);

  /** Compute the hash digest of an input range */
  template <typename InputIterator>
  std::string operator()(InputIterator first, InputIterator last);

  /** Return the size of the digest output in bytes */
  std::size_t size();

 private:
  std::unique_ptr<algorithm> algorithm_;
};

template <typename InputIterator, typename OutputIterator>
OutputIterator digest::operator()(InputIterator first, InputIterator last,
                                  OutputIterator output) {
  auto result = (*this)(first, last);
  std::copy(result.begin(), result.end(), output);
  return output;
}

template <typename InputIterator>
std::string digest::operator()(InputIterator first, InputIterator last) {
  algorithm_->reset();
  auto buffer = std::vector<char>{};
  auto chunk_size = algorithm_->block_size();
  buffer.reserve(chunk_size);
  while (first != last) {
    for (auto i = 0u; first != last && i < chunk_size; ++i, ++first) {
      buffer.emplace_back(*first);
    }
    auto view = std::string_view(buffer.data(), buffer.size());
    algorithm_->update(view);
    buffer.clear();
  }
  auto result = std::string(size(), 0);
  algorithm_->digest(result);
  return result;
}

}  // namespace multihash