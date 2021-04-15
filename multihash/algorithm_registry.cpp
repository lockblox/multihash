#include <cryptopp/sha.h>
#include <cryptopp/sha3.h>
#include <multihash/algorithm_registry.h>
#include <multihash/detail/cryptopp_impl.h>

namespace multihash {

const algorithm_registry& algorithm_registry::default_instance() {
  static auto default_registry =
      algorithm_registry{}
          .register_algorithm(
              algorithm_factory{algorithm_identifier::identity, "identity",
                                []() {
                                  return std::make_unique<
                                      detail::cryptopp_impl<CryptoPP::SHA1>>();
                                }})
          .register_algorithm(algorithm_factory{
              algorithm_identifier::sha2_256, "sha256",
              []() {
                return std::make_unique<
                    detail::cryptopp_impl<CryptoPP::SHA256>>();
              }})
          .register_algorithm(algorithm_factory{
              algorithm_identifier::sha2_512, "sha2-512",
              []() {
                return std::make_unique<
                    detail::cryptopp_impl<CryptoPP::SHA512>>();
              }})
          .register_algorithm(algorithm_factory{
              algorithm_identifier::sha3_256, "sha3-256", []() {
                return std::make_unique<
                    detail::cryptopp_impl<CryptoPP::SHA3_256>>();
              }});
  return default_registry;
}

}  // namespace multihash