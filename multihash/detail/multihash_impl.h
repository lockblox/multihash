#pragma once

#include <multihash/multihash.h>

namespace multihash {

class multihash::impl {
 public:
  impl() = default;
  impl(hash_code code, const std::string_view& digest);
  hash_code code() const;
  std::string_view digest() const;

  const char* data() const noexcept;
  std::size_t size() const noexcept;

  explicit operator std::vector<char>();

 private:
  std::vector<char> buffer_;
};

}  // namespace multihash
