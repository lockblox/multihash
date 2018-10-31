#pragma once

#include <multihash/HashType.h>
#include <memory>
#include <vector>

namespace multihash {

class Hash {
 public:
  Hash();
  Hash(HashType type, const std::vector<char>& digest);
  Hash& operator=(const Hash& rhs) noexcept;
  Hash(const Hash& rhs);
  ~Hash();

  const HashType& type() const;
  const std::vector<char>& digest() const;

  bool operator==(const Hash& rhs) const;
  bool operator!=(const Hash& rhs) const;
  bool operator<(const Hash& rhs) const;
  bool operator>(const Hash& rhs) const;

 private:
  class Impl;
  std::shared_ptr<Impl> pImpl;
};

std::ostream& operator<<(std::ostream& os, const Hash& hash);
}  // namespace multihash
