#pragma once

#include <cryptlib.h>
#include <multihash/detail/algorithm.h>
#include <multihash/hash_code.h>
#include <memory>

namespace multihash {

class cryptopp_impl : public algorithm {
 public:
  explicit cryptopp_impl(hash_code code);
  size_t block_size() override;
  void update(const std::string_view& data) override;
  std::vector<char> digest() override;

 private:
  std::unique_ptr<CryptoPP::HashTransformation> hash_;
  std::vector<char> digest_;
};
}  // namespace multihash
