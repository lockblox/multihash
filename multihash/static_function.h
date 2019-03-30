#pragma once

#include <cryptopp/sha.h>
#include <cryptopp/sha3.h>
#include <multihash/detail/cryptopp_impl.h>
#include <multihash/digest.h>

namespace multihash {

template <code_type HashCode, typename Algorithm>
class static_function : public digest {
 public:
  static constexpr code_type code = HashCode;
  static_function();
};

template <code_type HashCode, typename Algorithm>
static_function<HashCode, Algorithm>::static_function()
    : digest(std::make_unique<Algorithm>()) {}

using sha1 = static_function<code::sha1, cryptopp_impl<CryptoPP::SHA1>>;
using sha2_256 =
    static_function<code::sha2_256, cryptopp_impl<CryptoPP::SHA256>>;

using sha2_512 =
    static_function<code::sha2_512, cryptopp_impl<CryptoPP::SHA512>>;

using sha3_256 =
    static_function<code::sha3_256, cryptopp_impl<CryptoPP::SHA3_256>>;

}  // namespace multihash
