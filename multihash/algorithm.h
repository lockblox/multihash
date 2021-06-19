#pragma once

#include <cstddef>
#include <gsl/string_span>
#include <map>
#include <memory>
#include <string_view>
#include <vector>

namespace multihash {

using string_span = gsl::span<char>;

class algorithm {
 public:
  /** Compute the hash of an input range */
  template <typename InputIterator>
  auto digest(InputIterator first, InputIterator last) -> std::string;

  /** Write the hash of an input range into output */
  template <typename InputIterator, typename OutputIterator>
  auto digest(InputIterator first, InputIterator last,
              OutputIterator output) -> OutputIterator;

  virtual ~algorithm() = default;

 private:
  virtual void reset() = 0;
  virtual std::size_t digest_size() const = 0;
  virtual std::size_t block_size() const = 0;
  virtual void update(std::string_view data) = 0;
  virtual std::size_t digest(string_span output) = 0;
  std::string digest();


};

template <typename InputIterator>
auto algorithm::digest(InputIterator first, InputIterator last) -> std::string {
  auto result = std::string{};
  result.reserve(digest_size());
  digest(first, last, std::back_inserter(result));
  return result;
}

template <typename InputIterator, typename OutputIterator>
auto algorithm::digest(InputIterator first, InputIterator last,
                                     OutputIterator output) -> OutputIterator {
  reset();
  auto buffer = std::vector<char>{};
  auto chunk_size = block_size();
  buffer.reserve(chunk_size);
  while (first != last) {
    for (auto i = 0u; first != last && i < chunk_size; ++i, ++first) {
      buffer.emplace_back(*first);
    }
    auto view = std::string_view(buffer.data(), buffer.size());
    update(view);
    buffer.clear();
  }
  auto result = digest();
  std::copy(result.begin(), result.end(), output);
  return output;
}

}  // namespace multihash