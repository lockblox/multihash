#pragma once

#include <multihash/algorithm.h>

namespace multihash {

class digest {
 public:
  /** Create a hash by looking up an algorithm in the registry */
  explicit digest(std::unique_ptr<algorithm> algorithm);

  virtual ~digest() = default;

  /** Compute the hash of an input stream */
  std::string operator()(std::istream& input);

  /** Compute the hash of a string view */
  std::string operator()(string_view input);

  /** Write the hash of an input stream into a buffer */
  std::size_t operator()(std::istream& input, string_span output);

  /** Write the hash of a string view into a buffer */
  std::size_t operator()(string_view input, string_span output);

  /** Compare the hash of input with digest
   * @return true if hash matches the computed hash, false otherwise */
  bool verify(string_view input, string_view hash);

  std::size_t size();

 private:
  std::unique_ptr<algorithm> algorithm_;
};

}  // namespace multihash
