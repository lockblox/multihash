#pragma once

#include <multihash/multihash.h>

namespace multihash {

class multihash::impl {
 public:
  explicit impl(const std::string_view& buffer);
  impl(hash_code code, const std::string_view& digest);
  hash_code code() const;
  std::string_view digest() const;

  const char* data() const noexcept;
  std::size_t size() const noexcept;

  explicit operator std::vector<char>();

 private:
  void init(hash_code code, const std::string_view& digest);
  std::vector<char> buffer_;
};

}  // namespace multihash
