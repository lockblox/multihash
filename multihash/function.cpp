#include <multihash/function.h>

namespace multihash {

function::function(code_type code, bool multiformat)
    : code_(code),
      multiformat_(multiformat),
      algorithm_(algorithm::create(code_)) {}

code_type function::code() const { return code_; }

std::size_t function::size() {
  return multiformat_ ? ::multihash::size(code(), algorithm_->digest_size())
                      : algorithm_->digest_size();
}

}  // namespace multihash
