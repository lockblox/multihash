#include <multihash/hash_type.h>
#include <multihash/multihash.h>
#include <iomanip>
#include "multihash/detail/multihash_impl.h"

namespace multihash {

multihash::multihash() : pImpl(nullptr) {}

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
  pImpl.reset(new impl(buffer));
}

bool multihash::operator==(const multihash& rhs) const {
  if (pImpl == nullptr || rhs.pImpl == nullptr) {
    return pImpl == rhs.pImpl;
  }
  return code() == rhs.code() && digest() == rhs.digest();
}

bool multihash::operator!=(const multihash& rhs) const {
  return !(*this == rhs);
}

bool multihash::operator<(const multihash& rhs) const {
  if (pImpl == nullptr || rhs.pImpl == nullptr) {
    return pImpl < rhs.pImpl;
  }
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
