#include <multihash/hash_type.h>
#include <multihash/multihash.h>
#include <iomanip>
#include "multihash/detail/multihash_impl.h"

namespace multihash {

multihash::multihash() : pImpl(new impl()) {}

multihash::multihash(hash_code code, const std::string_view& digest)
    : pImpl(new impl(code, digest)) {}

multihash::multihash(const multihash& rhs) : pImpl(rhs.pImpl) {}

multihash& multihash::operator=(const multihash& rhs) noexcept {
  auto copy = rhs;
  std::swap(pImpl, copy.pImpl);
  return *this;
}

multihash::~multihash() {}

multihash::multihash(const std::string_view& buffer) {
  auto code_byte = uint8_t(buffer.at(0));
  if (code_byte > 127) {
    throw std::invalid_argument("Unsupported hash code > 127");
  }
  auto code = static_cast<hash_code>(code_byte);
  auto size = uint8_t(buffer.at(1));
  if (size > 127) {
    throw std::invalid_argument("Unsupported digest size > 127");
  }
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
  pImpl.reset(new impl(code, digest));
}

bool multihash::operator==(const multihash& rhs) const {
  return code() == rhs.code() && digest() == rhs.digest();
}

bool multihash::operator!=(const multihash& rhs) const {
  return (code() != rhs.code()) || (digest() != rhs.digest());
}

bool multihash::operator<(const multihash& rhs) const {
  auto result = false;
  if (code() == rhs.code()) {
    result = digest() < rhs.digest();
  } else {
    result = code() < rhs.code();
  }
  return result;
}

bool multihash::operator>(const multihash& rhs) const {
  auto result = false;
  if (code() == rhs.code()) {
    result = digest() > rhs.digest();
  } else {
    result = code() > rhs.code();
  }
  return result;
}

hash_code multihash::code() const { return pImpl->code(); }

std::string_view multihash::digest() const { return pImpl->digest(); }

const char* multihash::data() const { return pImpl->data(); }

std::size_t multihash::size() const { return pImpl->size(); }

multihash::operator std::vector<char>() {
  return static_cast<std::vector<char>>(*pImpl);
}

std::ostream& operator<<(std::ostream& os, const multihash& hash) {
  auto data = std::string_view(hash.data(), hash.size());
  for (auto c : data) {
    auto uc = uint8_t(c);
    os << std::hex << std::setfill('0') << std::setw(2);
    os << static_cast<int>(uc);
  }
  return os;
}
}  // namespace multihash
