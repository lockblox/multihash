#include <cryptopp/sha.h>
#include <cryptopp/sha3.h>
#include <multihash/algorithm.h>
#include <multihash/code.h>
#include <multihash/detail/cryptopp_impl.h>
#include "detail/cryptopp_impl.h"

namespace multihash {

std::map<varint_view, algorithm::factory*> algorithm::factories() {
  static auto result = std::map<varint_view, algorithm::factory*>{
      {code::sha1, &detail::cryptopp_factory<CryptoPP::SHA1>::instance()},
      {code::sha2_256, &detail::cryptopp_factory<CryptoPP::SHA256>::instance()},
      {code::sha2_512, &detail::cryptopp_factory<CryptoPP::SHA512>::instance()},
      {code::sha3_256,
       &detail::cryptopp_factory<CryptoPP::SHA3_256>::instance()}};
  return result;
}

std::unique_ptr<algorithm> algorithm::create(varint_view code) {
  auto factory = factories().at(code);
  assert(factory);
  return factory->create();
}

}  // namespace multihash
