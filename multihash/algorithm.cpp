#include <cryptopp/sha.h>
#include <cryptopp/sha3.h>
#include <multihash/algorithm.h>
#include <multihash/detail/cryptopp_impl.h>
#include "detail/cryptopp_impl.h"

namespace multihash {

std::string algorithm::digest() {
  auto result = std::string(digest_size(), 0);
  auto span = string_span(result);
  digest(span);
  return result;
}

}  // namespace multihash
