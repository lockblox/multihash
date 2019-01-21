#include "multihash_impl.h"
#include <multihash/hash_type.h>

namespace multihash {
multihash::impl::impl(const std::string_view& buffer) {
  auto code_byte = uint8_t(buffer.at(0));
  if (code_byte > 127) {
    throw std::invalid_argument("Unsupported hash code > 127");
  }
  auto code = static_cast<hash_code>(code_byte);
  auto size = uint8_t(buffer.at(1));
  auto type = hash_type(code);
  auto digest = std::string_view(&(buffer.at(2)), buffer.size() - 2);
  if (digest.size() != size) {
    std::string msg = "Header misreports digest size as " +
                      std::to_string(size) + "; true size is " +
                      std::to_string(digest.size());
    throw std::out_of_range(msg);
  }
  if (size != type.size()) {
    std::string msg = "Non standard digest size " + std::to_string(size) +
                      "; expected " + std::to_string(type.size());
    throw std::out_of_range(msg);
  }
  init(code, digest);
}

multihash::impl::impl(hash_code code, const std::string_view& digest) {
  init(code, digest);
}

void multihash::impl::init(hash_code code, const std::string_view& digest) {
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
