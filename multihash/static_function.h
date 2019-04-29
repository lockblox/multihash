#pragma once

#include <cryptopp/sha.h>
#include <cryptopp/sha3.h>
#include <multihash/detail/cryptopp_impl.h>
#include <multihash/digest.h>

namespace multihash {

template <std::size_t HashCode, typename Algorithm>
class static_function : public digest {
 public:
  static_function();
};

template <std::size_t HashCode, typename Algorithm>
static_function<HashCode, Algorithm>::static_function()
    : digest(std::make_unique<Algorithm>()) {}

using sha1 = static_function<0x11, detail::cryptopp_impl<CryptoPP::SHA1>>;
using sha2_256 = static_function<0x12, detail::cryptopp_impl<CryptoPP::SHA256>>;
using sha2_512 = static_function<0x13, detail::cryptopp_impl<CryptoPP::SHA512>>;
using sha3_256 =
    static_function<0x16, detail::cryptopp_impl<CryptoPP::SHA3_256>>;

}  // namespace multihash
