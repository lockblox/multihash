#pragma once

#include <multihash/Hash.h>

namespace multihash {

class Hash::Impl {
 public:
  Impl(HashType type, std::vector<char> digest);
  const HashType& type() const;
  const std::vector<char>& digest() const;

 private:
  const HashType type_;
  const std::vector<char> digest_;
};
}  // namespace multihash
