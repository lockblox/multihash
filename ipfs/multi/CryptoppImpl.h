#pragma once

#include <cryptopp/cryptlib.h>
#include <ipfs/multi/Algorithm.h>
#include <ipfs/multi/HashType.h>
#include <memory>

namespace ipfs {
namespace multi {

class CryptoppImpl : public Algorithm {
 public:
  explicit CryptoppImpl(const HashType& hash_type);
  size_t block_size() override;
  void update(const string_view& data) override;
  std::vector<char> digest() override;

 private:
  std::unique_ptr<CryptoPP::HashTransformation> hash_;
  std::vector<char> digest_;
};
}  // namespace multi
}  // namespace ipfs
