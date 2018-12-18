#include "multihash_impl.h"
#include <multihash/hash_type.h>

namespace multihash {

multihash::impl::impl(hash_code code, const std::string_view& digest) {
  if (digest.size() > 127) {
    throw std::invalid_argument("Unsupported digest size > 127");
  }
  buffer_.reserve(digest.size() + 2);
  buffer_.push_back(static_cast<char>(code));
  buffer_.push_back(static_cast<char>(digest.size()));
  std::copy(digest.begin(), digest.end(), std::back_inserter(buffer_));
}

hash_code multihash::impl::code() const {
  return static_cast<hash_code>(buffer_[0]);
}

std::string_view multihash::impl::digest() const {
  auto view = std::string_view(&buffer_[2], buffer_.size() - 2);
  return view;
}

const char* multihash::impl::data() const noexcept { return &buffer_[0]; }

std::size_t multihash::impl::size() const noexcept { return buffer_.size(); }

multihash::impl::operator std::vector<char>() { return buffer_; }

}  // namespace multihash
