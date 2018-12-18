#include <algorithm>
#include <sstream>
#include "multihash/detail/hash_type_impl.h"

namespace multihash {

hash_type::hash_type() : hash_type(hash_code::IDENTITY) {}

hash_type::hash_type(const std::string& name) {
  auto all_types(impl::all_types());
  auto end(std::end(all_types));
  auto it(std::find_if(std::begin(all_types), end,
                       [name](const hash_type::impl* elem) -> bool {
                         return name == elem->name();
                       }));
  if (end == it) {
    throw std::invalid_argument("Invalid hash name: " + name);
  }
  pImpl = *it;
}

hash_type::hash_type(hash_code code) {
  auto all_types(impl::all_types());
  auto end(std::end(all_types));
  auto it(std::find_if(std::begin(all_types), end,
                       [code](const hash_type::impl* elem) -> bool {
                         return code == elem->code();
                       }));
  if (end == it) {
    auto string_code = std::to_string(static_cast<int>(code));
    throw std::invalid_argument("Invalid hash code: " + string_code);
  }
  pImpl = *it;
}

hash_type::hash_type(const hash_type& rhs) : pImpl(rhs.pImpl) {}

const std::string& hash_type::name() const { return pImpl->name(); }

hash_code hash_type::code() const { return pImpl->code(); }

size_t hash_type::size() const { return pImpl->size(); }

bool hash_type::operator==(const hash_type& rhs) const {
  return pImpl == rhs.pImpl;
}

bool hash_type::operator!=(const hash_type& rhs) const {
  return pImpl != rhs.pImpl;
}

bool hash_type::operator<(const hash_type& rhs) const {
  return code() < rhs.code();
}

bool hash_type::operator>(const hash_type& rhs) const {
  return code() > rhs.code();
}

std::set<hash_type> hash_type::types() {
  return std::set<hash_type>{hash_type{hash_code::SHA1},
                             hash_type{hash_code::SHA2_256},
                             hash_type{hash_code::SHA2_512}};
}

std::ostream& operator<<(std::ostream& os, const hash_type& hash_type) {
  os << hash_type.name();
  return os;
}

}  // namespace multihash
