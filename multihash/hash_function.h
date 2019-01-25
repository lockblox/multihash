#pragma once

#include <multihash/multihash.h>
#include <istream>

namespace multihash {

/** Functor which hashes an input stream with a given hash type */
class hash_function {
 public:
  using result_type = multihash;
  using argument_type = std::string_view;

  explicit hash_function(hash_code code = hash_code::SHA2_256);
  hash_function(const hash_function& other);
  hash_function& operator=(const hash_function& rhs);
  ~hash_function();

  bool operator==(const hash_function& rhs) const;

  multihash operator()(std::istream& input) const;
  multihash operator()(std::string_view input) const;
  const hash_code& code() const;

 private:
  class impl;

  std::unique_ptr<impl> pImpl;
};
}  // namespace multihash
