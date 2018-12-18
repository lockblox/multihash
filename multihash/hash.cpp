#include "multihash/detail/hash_impl.h"

namespace multihash {

hash::hash(hash_code code) : pImpl(new impl(code)) {}

hash::hash(const hash& other) : pImpl(new impl(other.code())) {}

hash& hash::operator=(const hash& rhs) {
  if (!(code() == rhs.code())) {
    pImpl.reset(new impl(hash_code(rhs.code())));
  }
  return *this;
}

/** Though the destructor is trivial, we must define it separately since the
 * default destructor of unique_ptr would require a complete declaration of
 * Impl
 */
hash::~hash() {}

multihash hash::operator()(std::istream& input) const {
  return (*pImpl)(input);
}

multihash hash::operator()(const std::string_view input) const {
  return (*pImpl)(input);
}

const hash_code& hash::code() const { return pImpl->code(); }
}  // namespace multihash
