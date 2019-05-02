#include <multihash/function.h>

namespace multihash {

function::function(varint_view code)
    : code_(std::move(code)), digest_(algorithm::create(code)) {}

varint_view function::code() const { return code_; }

std::size_t function::size() {
  auto digest_size = digest_.size();
  return ::multihash::size(code(), digest_size);
}

}  // namespace multihash
