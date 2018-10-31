#include "CryptoppImpl.h"
#include <blake2.h>
#include <sha.h>
#include <sha3.h>

namespace multihash {

CryptoppImpl::CryptoppImpl(const HashType& hash_type) {
  switch (hash_type.code()) {
    case HashCode::SHA1:
      hash_ = std::make_unique<CryptoPP::SHA1>();
      digest_.resize(CryptoPP::SHA1::DIGESTSIZE);
      break;
    case HashCode::SHA2_256:
      hash_ = std::make_unique<CryptoPP::SHA256>();
      digest_.resize(CryptoPP::SHA256::DIGESTSIZE);
      break;
    case HashCode::SHA2_512:
      hash_ = std::make_unique<CryptoPP::SHA512>();
      digest_.resize(CryptoPP::SHA512::DIGESTSIZE);
      break;
    case HashCode::SHA3:
      hash_ = std::make_unique<CryptoPP::SHA3_256>();
      digest_.resize(CryptoPP::SHA3_256::DIGESTSIZE);
      break;
    case HashCode::BLAKE2B:
      hash_ = std::make_unique<CryptoPP::BLAKE2b>();
      digest_.resize(CryptoPP::BLAKE2b::DIGESTSIZE);
      break;
    case HashCode::BLAKE2S:
      hash_ = std::make_unique<CryptoPP::BLAKE2s>();
      digest_.resize(CryptoPP::BLAKE2s::DIGESTSIZE);
      break;
    case HashCode::IDENTITY:
      auto name = hash_type.name();
      throw std::invalid_argument("Unsupported hash function" + name);
  }
}

size_t CryptoppImpl::block_size() { return hash_->OptimalBlockSize(); }

void CryptoppImpl::update(const string_view& data) {
  hash_->Update(reinterpret_cast<const unsigned char*>(data.data()),
                data.size());
}

std::vector<char> CryptoppImpl::digest() {
  // TODO clone so that we can continue updating
  hash_->Final(reinterpret_cast<unsigned char*>(digest_.data()));
  return digest_;
}
}  // namespace multihash
