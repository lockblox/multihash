#include <multihash/function.h>

namespace multihash {

function::function(code_type code)
    : code_(std::move(code)), digest_(algorithm::create(code)) {}

code_type function::code() const { return code_; }

std::size_t function::size() {
  auto digest_size = digest_.size();
  return ::multihash::size(code(), digest_size);
}

}  // namespace multihash
