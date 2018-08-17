#include "HashImpl.h"

namespace multihash {

Hash::Impl::Impl(HashType type, std::vector<char> digest)
    : type_(std::move(type)), digest_(std::move(digest)) {}
const HashType &Hash::Impl::type() const { return type_; }
const std::vector<char> &Hash::Impl::digest() const { return digest_; }

}  // namespace multihash
