#include <multihash/function.h>

namespace multihash {

function::function(algorithm::code_type code)
    : code_(code), digest_(algorithm::create(code)) {}

algorithm::code_type function::code() const { return code_; }

std::size_t function::size() {
  using multihash = multihash<std::string>;
  auto digest_size = digest_.size();
  return multihash::size(code(), digest_size);
}

}  // namespace multihash
