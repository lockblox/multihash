#include <algorithm>
#include <sstream>
#include "multihash/detail/HashTypeImpl.h"

namespace multihash {

HashType::HashType() : HashType(HashCode::IDENTITY) {}

HashType::HashType(const std::string& name) {
  auto all_types(Impl::all_types());
  auto end(std::end(all_types));
  auto it(std::find_if(std::begin(all_types), end,
                       [name](const HashType::Impl* elem) -> bool {
                         return name == elem->name();
                       }));
  if (end == it) {
    throw std::invalid_argument("Invalid hash name: " + name);
  }
  pImpl = *it;
}

HashType::HashType(HashCode code) {
  auto all_types(Impl::all_types());
  auto end(std::end(all_types));
  auto it(std::find_if(std::begin(all_types), end,
                       [code](const HashType::Impl* elem) -> bool {
                         return code == elem->code();
                       }));
  if (end == it) {
    auto string_code = std::to_string(static_cast<int>(code));
    throw std::invalid_argument("Invalid hash code: " + string_code);
  }
  pImpl = *it;
}

HashType::HashType(const HashType& rhs) : pImpl(rhs.pImpl) {}

const std::string& HashType::name() const { return pImpl->name(); }

HashCode HashType::code() const { return pImpl->code(); }

size_t HashType::size() const { return pImpl->size(); }

bool HashType::operator==(const HashType& rhs) const {
  return pImpl == rhs.pImpl;
}

bool HashType::operator!=(const HashType& rhs) const {
  return pImpl != rhs.pImpl;
}

bool HashType::operator<(const HashType& rhs) const {
  return code() < rhs.code();
}

bool HashType::operator>(const HashType& rhs) const {
  return code() > rhs.code();
}

std::set<HashType> HashType::types() {
  return std::set<HashType>{HashType{HashCode::SHA1},
                            HashType{HashCode::SHA2_256},
                            HashType{HashCode::SHA2_512}};
}

std::ostream& operator<<(std::ostream& os, const HashType& hash_type) {
  os << hash_type.name();
  return os;
}

std::ostream& operator<<(std::ostream& os, const HashCode& hash_code) {
  os << std::hex << static_cast<int>(hash_code);
  return os;
}
}  // namespace multihash
