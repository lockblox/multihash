#include "hash_impl.h"
#include "cryptopp_impl.h"

namespace multihash {
hash::impl::impl(hash_code code) : hash_code_(code) { init(); }

void hash::impl::init() {
  switch (hash_code_) {
    case hash_code::SHA1:
    case hash_code::SHA2_256:
    case hash_code::SHA2_512:
      algorithm_ = std::make_unique<cryptopp_impl>(hash_code_);
      break;
    case hash_code::SHA3:
    case hash_code::IDENTITY:
      std::string msg = "No hash function for code " +
                        std::to_string(static_cast<char>(hash_code_));
      throw std::out_of_range(msg);
  }
}

bool hash::operator==(const hash& rhs) const { return code() == rhs.code(); }

multihash hash::impl::operator()(std::istream& input) {
  if (!input.good()) {
    throw std::invalid_argument("hash input is not good");
  }
  init();
  auto buffer = std::string(algorithm_->block_size(), ' ');
  auto begin = buffer.begin();
  auto end = buffer.end();

  while (!input.eof()) {
    input.read(const_cast<char*>(buffer.data()), buffer.size());

    if (!input.eof()) {
      // filled the buffer so update the hash
      algorithm_->update(buffer);
    }
  }
  // final update to hash with partially filled vector
  auto chars_read(input.gcount());
  std::advance(begin, chars_read);
  buffer.erase(begin, end);
  algorithm_->update(buffer);
  return digest();
}

multihash hash::impl::operator()(std::string_view input) {
  init();
  auto block_size = algorithm_->block_size();
  auto begin = input.begin();
  auto size = std::min(input.size(), block_size);
  auto remaining = input.size();
  auto end = begin;

  while (size > 0) {
    std::advance(end, size);
    auto buffer = std::string_view(input.data(), size);
    algorithm_->update(buffer);
    begin = end;
    remaining -= size;
    size = std::min(remaining, block_size);
  }
  return digest();
}

multihash hash::impl::digest() {
  auto digest = algorithm_->digest();
  auto view = std::string_view(digest.data(), digest.size());
  return multihash(hash_code_, view);
}

}  // namespace multihash
