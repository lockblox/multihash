#include <cryptopp/sha.h>
#include <cryptopp/sha3.h>
#include <multihash/algorithm_registry.h>
#include <multihash/detail/cryptopp_impl.h>

namespace multihash {

const algorithm_registry& algorithm_registry::default_instance() {
  static const auto default_registry =
      algorithm_registry{}
          .register_algorithm(
              algorithm_factory{algorithm_identifier::sha1, "sha1",
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

algorithm_registry::const_iterator algorithm_registry::begin() const {
  return algorithm_factories_.begin();
}

algorithm_registry::const_iterator algorithm_registry::end() const {
  return algorithm_factories_.end();
}

algorithm_registry& algorithm_registry::register_algorithm(
    const algorithm_factory& factory) {
  algorithm_factories_.push_back(factory);
  return *this;
}

algorithm_registry::const_iterator algorithm_registry::find(
    const std::string& algorithm_name) const {
  auto it = std::find_if(begin(), end(), [algorithm_name](auto& factory) {
    return factory.name() == algorithm_name;
  });
  return it;
}

algorithm_registry::const_iterator algorithm_registry::find(
    algorithm_identifier_type algorithm_identifier) const {
  auto it =
      std::find_if(begin(), end(), [algorithm_identifier](auto& factory) {
        return factory.identifier() == algorithm_identifier;
      });
  return it;
}

const algorithm_factory& algorithm_registry::at(
    const std::string& algorithm_name) const {
  auto factory = find(algorithm_name);
  if (factory == end()) {
    throw std::out_of_range("No such algorithm");
  }
  return *factory;
}

const algorithm_factory& algorithm_registry::at(
    algorithm_identifier_type algorithm_identifier) const {
  auto factory = find(algorithm_identifier);
  if (factory == end()) {
    throw std::out_of_range("No such algorithm");
  }
  return *factory;
}

std::unique_ptr<algorithm> algorithm_registry::make_algorithm(
    const std::string& algorithm_name) const {
  return at(algorithm_name).make_algorithm();
}

std::unique_ptr<algorithm> algorithm_registry::make_algorithm(
    algorithm_identifier_type algorithm_identifier) const {
  return at(algorithm_identifier).make_algorithm();
}

}  // namespace multihash