#pragma once

#include <multihash/multihash.h>
#include <istream>

namespace multihash {

/** Functor which hashes an input stream with a given hash type */
class hash {
 public:
  using result_type = multihash;
  using argument_type = std::string_view;

  explicit hash(hash_code code = hash_code::SHA2_256);
  hash(const hash& other);
  hash& operator=(const hash& rhs);
  ~hash();

  bool operator==(const hash& rhs) const;

  multihash operator()(std::istream& input) const;
  multihash operator()(std::string_view input) const;
  const hash_code& code() const;

 private:
  class impl;

  std::unique_ptr<impl> pImpl;
};
}  // namespace multihash
