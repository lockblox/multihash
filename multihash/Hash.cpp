#include <multihash/HashRawCodec.h>
#include <iomanip>
#include "HashImpl.h"

namespace multihash {

Hash::Hash() : pImpl(new Impl(HashType(), std::vector<char>())) {}

Hash::Hash(HashType type, const std::vector<char> &digest)
    : pImpl(new Impl(type, digest)) {}

Hash::Hash(const Hash &rhs) : pImpl(rhs.pImpl) {}

Hash &Hash::operator=(const Hash &rhs) noexcept {
  auto copy = rhs;
  std::swap(pImpl, copy.pImpl);
  return *this;
}

Hash::~Hash() {}

bool Hash::operator==(const Hash &rhs) const {
  return type() == rhs.type() && digest() == rhs.digest();
}

bool Hash::operator!=(const Hash &rhs) const {
  return (type() != rhs.type()) || (digest() != rhs.digest());
}

bool Hash::operator<(const Hash &rhs) const {
  auto result = false;
  if (type() == rhs.type()) {
    result = digest() < rhs.digest();
  } else {
    result = type() < rhs.type();
  }
  return result;
}

bool Hash::operator>(const Hash &rhs) const {
  auto result = false;
  if (type() == rhs.type()) {
    result = digest() > rhs.digest();
  } else {
    result = type() > rhs.type();
  }
  return result;
}

const HashType &Hash::type() const { return pImpl->type(); }

const std::vector<char> &Hash::digest() const { return pImpl->digest(); }

std::ostream &operator<<(std::ostream &os, const Hash &hash) {
  HashRawCodec encode;
  auto data(encode(hash));
  for (auto c : data) {
    auto uc = uint8_t(c);
    os << std::hex << std::setfill('0') << std::setw(2);
    os << static_cast<int>(uc);
  }
  return os;
}

}  // namespace Hash
