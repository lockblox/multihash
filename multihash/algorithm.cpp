#include "algorithm.h"
#include <cryptopp/sha.h>
#include <cryptopp/sha3.h>
#include <multihash/algorithm.h>
#include <multihash/detail/cryptopp_impl.h>
#include "detail/cryptopp_impl.h"

namespace multihash {

std::map<algorithm::code_type, algorithm::factory*> algorithm::factories_ = {
    {algorithm::code::sha1, &cryptopp_factory<CryptoPP::SHA1>::instance()},
    {algorithm::code::sha2_256,
     &cryptopp_factory<CryptoPP::SHA256>::instance()},
    {algorithm::sha2_512, &cryptopp_factory<CryptoPP::SHA512>::instance()},
    {algorithm::code::sha3_256,
     &cryptopp_factory<CryptoPP::SHA3_256>::instance()}};

std::map<algorithm::code_type, std::string> algorithm::codenames_ = {
    {algorithm::code::sha1, "sha1"},
    {algorithm::code::sha2_256, "sha2-256"},
    {algorithm::code::sha2_512, "sha2-512"},
    {algorithm::code::sha3_256, "sha3-256"}};

std::unique_ptr<algorithm> algorithm::create(algorithm::code_type code) {
  auto factory = factories_.at(code);
  assert(factory);
  return factory->create();
}

void algorithm::register_factory(algorithm::codename codename,
                                 algorithm::factory& factory) {
  codenames_[codename.first] = codename.second;
  factories_[codename.first] = &factory;
}

algorithm::code_type algorithm::code(std::string name) {
  auto it = std::find_if(codenames_.begin(), codenames_.end(),
                         [&name](auto i) { return i.second == name; });
  if (it == codenames_.end()) {
    throw std::out_of_range("Unknown code " + name);
  }
  return it->first;
}

std::string algorithm::name(algorithm::code_type code) {
  return codenames_.at(code);
}

void algorithm::register_codename(algorithm::codename codename) {
  codenames_[codename.first] = codename.second;
}

const std::map<algorithm::code_type, std::string>& algorithm::codenames() {
  return codenames_;
}

}  // namespace multihash
