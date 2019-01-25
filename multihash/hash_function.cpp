#include "multihash/detail/hash_function_impl.h"

namespace multihash {

hash_function::hash_function(hash_code code) : pImpl(new impl(code)) {}

hash_function::hash_function(const hash_function& other) : pImpl(new impl(other.code())) {}

hash_function& hash_function::operator=(const hash_function& rhs) {
  if (!(code() == rhs.code())) {
    pImpl.reset(new impl(hash_code(rhs.code())));
  }
  return *this;
}

/** Though the destructor is trivial, we must define it separately since the
 * default destructor of unique_ptr would require a complete declaration of
 * Impl
 */
hash_function::~hash_function() {}

multihash hash_function::operator()(std::istream& input) const {
  return (*pImpl)(input);
}

multihash hash_function::operator()(const std::string_view input) const {
  return (*pImpl)(input);
}

const hash_code& hash_function::code() const { return pImpl->code(); }
}  // namespace multihash
