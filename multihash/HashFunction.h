#pragma once

#include <multihash/Hash.h>
#include <multihash/StringView.h>
#include <istream>

namespace multihash {

/** Interface for a hash digest with encapsulated type */
/** Functor which hashes an input stream with a given hash type */
class HashFunction {
 public:
  using result_type = Hash;
  using argument_type = string_view;

  explicit HashFunction(HashCode code = HashCode::SHA2_256);
  explicit HashFunction(const HashType& hash_type);
  explicit HashFunction(const std::string& hash_type);
  HashFunction(const HashFunction& other);
  HashFunction& operator=(const HashFunction& rhs);
  ~HashFunction();

  bool operator==(const HashFunction& rhs) const;

  Hash operator()(std::istream& input) const;
  Hash operator()(string_view input) const;
  const HashType& type() const;

 private:
  class Impl;

  std::unique_ptr<Impl> pImpl;
};
}  // namespace multihash
