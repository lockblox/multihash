#include "digest.h"
#include <multihash/digest.h>

namespace multihash {

digest::digest(std::unique_ptr<algorithm> algorithm)
    : algorithm_(std::move(algorithm)) {}

std::size_t digest::size() { return algorithm_->digest_size(); }

}  // namespace multihash