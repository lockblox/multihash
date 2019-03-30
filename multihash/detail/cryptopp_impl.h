#pragma once

#include <multihash/algorithm.h>
#include <algorithm>
#include <cassert>
#include <cctype>

namespace multihash {

template <typename Hash>
class cryptopp_impl : public algorithm {
 public:
  void reset() override;
  size_t digest_size() const override;
  size_t block_size() const override;
  void update(string_view input) override;
  std::size_t digest(string_span output) override;

 private:
  Hash hash_;
};

template <typename Hash>
class cryptopp_factory : public algorithm::factory {
 public:
  std::unique_ptr<algorithm> create() override;
  static cryptopp_factory& instance();
};

template <typename Hash>
std::unique_ptr<algorithm> cryptopp_factory<Hash>::create() {
  return std::make_unique<cryptopp_impl<Hash>>();
}

template <typename Hash>
cryptopp_factory<Hash>& cryptopp_factory<Hash>::instance() {
  static cryptopp_factory instance_;
  return instance_;
}

template <typename Hash>
size_t cryptopp_impl<Hash>::digest_size() const {
  return Hash::DIGESTSIZE;
}

template <typename Hash>
size_t cryptopp_impl<Hash>::block_size() const {
  return hash_.OptimalBlockSize();
}

template <typename Hash>
void cryptopp_impl<Hash>::update(string_view input) {
  assert(input.data() != nullptr);
  assert(!input.empty());
  hash_.Update(reinterpret_cast<const unsigned char*>(input.data()),
               input.size());
}

template <typename Hash>
std::size_t cryptopp_impl<Hash>::digest(string_span output) {
  hash_.Final(reinterpret_cast<unsigned char*>(output.data()));
  return digest_size();
}

template <typename Hash>
void cryptopp_impl<Hash>::reset() {
  hash_.Restart();
}

}  // namespace multihash
