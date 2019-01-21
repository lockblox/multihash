#pragma once

#include <multihash/hash_code.h>
#include <memory>
#include <vector>

namespace multihash {

class multihash {
 public:
  multihash();
  explicit multihash(const std::string_view& buffer);
  multihash(hash_code code, const std::string_view& digest);
  multihash& operator=(const multihash& rhs) noexcept;
  multihash(const multihash& rhs);
  ~multihash();

  hash_code code() const;
  std::string_view digest() const;

  explicit operator std::vector<char>();

  const char* data() const;
  std::size_t size() const;

  bool operator==(const multihash& rhs) const;
  bool operator!=(const multihash& rhs) const;
  bool operator<(const multihash& rhs) const;
  bool operator>(const multihash& rhs) const;

 private:
  class impl;
  std::shared_ptr<impl> pImpl;
};

std::ostream& operator<<(std::ostream& os, const multihash& hash);
}  // namespace multihash
