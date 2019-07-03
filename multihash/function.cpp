#include <multihash/function.h>

namespace multihash {

function::function(varint_view code, bool multiformat)
    : code_(code),
      multiformat_(multiformat),
      algorithm_(algorithm::create(code_)) {}

varint_view function::code() const { return code_; }

std::size_t function::size() {
  return multiformat_ ? ::multihash::size(code(), algorithm_->digest_size())
                      : algorithm_->digest_size();
}

}  // namespace multihash
