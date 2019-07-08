#pragma once
#include <multihash/digest.h>
#include <cassert>

namespace multihash {

class function {
 public:
  /** Creates a multihash function
   *
   * @param code Unique identifier for the hash function
   * @param multiformat If true, include hash ID in the output
   */
  explicit function(code_type code = code::sha2_256, bool multiformat = true);

  /** Compute the hash of an input range */
  template <typename InputIterator>
  digest<std::string> operator()(InputIterator first, InputIterator last);

  /** Write the hash of an input range into output */
  template <typename InputIterator, typename OutputIterator>
  OutputIterator operator()(InputIterator first, InputIterator last,
                            OutputIterator output);

  /** Returns the code of this hash */
  code_type code() const;

  /** Returns the size of the multihash */
  std::size_t size();

 private:
  code_type code_;
  bool multiformat_;
  std::unique_ptr<algorithm> algorithm_;
};

template <typename InputIterator>
digest<std::string> function::operator()(InputIterator first,
                                         InputIterator last) {
  auto result = std::string();
  result.reserve(size());
  (*this)(first, last, std::back_inserter(result));
  return digest<std::string>{result};
}

template <typename InputIterator, typename OutputIterator>
OutputIterator function::operator()(InputIterator first, InputIterator last,
                                    OutputIterator output) {
  if (multiformat_) output = write(code(), algorithm_->digest_size(), output);
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
  auto result = algorithm_->digest();
  std::copy(result.begin(), result.end(), output);
  return output;
}

}  // namespace multihash
