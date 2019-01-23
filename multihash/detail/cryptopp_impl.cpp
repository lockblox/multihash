#include "cryptopp_impl.h"
#include <cassert>
#include <multihash/hash_type.h>
#include <sha.h>
#include <sha3.h>

namespace multihash {

cryptopp_impl::cryptopp_impl(hash_code code) {
  switch (code) {
    case hash_code::SHA1:
      hash_ = std::make_unique<CryptoPP::SHA1>();
      digest_.resize(CryptoPP::SHA1::DIGESTSIZE);
      break;
    case hash_code::SHA2_256:
      hash_ = std::make_unique<CryptoPP::SHA256>();
      digest_.resize(CryptoPP::SHA256::DIGESTSIZE);
      break;
    case hash_code::SHA2_512:
      hash_ = std::make_unique<CryptoPP::SHA512>();
      digest_.resize(CryptoPP::SHA512::DIGESTSIZE);
      break;
    case hash_code::SHA3_256:
      hash_ = std::make_unique<CryptoPP::SHA3_256>();
      digest_.resize(CryptoPP::SHA3_256::DIGESTSIZE);
      break;
    case hash_code::IDENTITY:
      throw std::invalid_argument("Unsupported hash code " +
                                  std::to_string(static_cast<char>(code)));
  }
}

size_t cryptopp_impl::block_size() { return hash_->OptimalBlockSize(); }

void cryptopp_impl::update(const std::string_view& data) {
  assert(data.data() != nullptr);
  assert(data.size() > 0);
  hash_->Update(reinterpret_cast<const unsigned char*>(data.data()),
                data.size());
}

std::vector<char> cryptopp_impl::digest() {
  // TODO clone so that we can continue updating
  hash_->Final(reinterpret_cast<unsigned char*>(digest_.data()));
  return digest_;
}
}  // namespace multihash
