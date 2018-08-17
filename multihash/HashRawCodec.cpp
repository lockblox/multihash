#include "HashRawCodecImpl.h"

namespace multihash {

HashRawCodec::HashRawCodec() : pImpl(HashRawCodec::Impl::instance()) {}

HashRawCodec::HashRawCodec(const HashRawCodec &other) : pImpl(other.pImpl) {}

HashRawCodec &HashRawCodec::operator=(const HashRawCodec &rhs) {
  pImpl = rhs.pImpl;
  return *this;
}

HashRawCodec::~HashRawCodec() {}

std::vector<char> HashRawCodec::operator()(const Hash &hash) const {
  return pImpl->encode(hash);
}

Hash HashRawCodec::operator()(const std::vector<char> &input) const {
  return pImpl->decode(input);
}

}  // namespace multihash
