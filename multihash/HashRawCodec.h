#pragma once

#include <multihash/Hash.h>
#include <vector>

namespace multihash {

/** Functor which converts a hash to and from raw bytes */
class HashRawCodec {
 public:
  using encoded_type = std::vector<char>;
  using value_type = Hash;

  HashRawCodec();
  HashRawCodec(const HashRawCodec& other);
  HashRawCodec& operator=(const HashRawCodec& rhs);
  ~HashRawCodec();

  std::vector<char> operator()(const Hash& hash) const;
  Hash operator()(const std::vector<char>& input) const;

 private:
  class Impl;
  const Impl* pImpl;
};
}  // namespace multihash
