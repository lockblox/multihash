#include "hash_type_impl.h"

namespace multihash {

hash_type::impl::impl(hash_code code, std::string name, size_t size)
    : code_(code), name_(std::move(name)), size_(size) {}

const hash_type::impl hash_type::impl::identity =
    hash_type::impl(hash_code::IDENTITY, "identity", 0);
const hash_type::impl hash_type::impl::sha1 =
    hash_type::impl(hash_code::SHA1, "sha1", 20);
const hash_type::impl hash_type::impl::sha2_256 =
    hash_type::impl(hash_code::SHA2_256, "sha2-256", 32);
const hash_type::impl hash_type::impl::sha2_512 =
    hash_type::impl(hash_code::SHA2_512, "sha2-512", 64);
const hash_type::impl hash_type::impl::sha3_256 =
    hash_type::impl(hash_code::SHA3_256, "sha3", 32);

const hash_type::impl::Set hash_type::impl::all_types() {
  return hash_type::impl::Set{
      &hash_type::impl::identity, &hash_type::impl::sha1,
      &hash_type::impl::sha2_256, &hash_type::impl::sha2_512,
      &hash_type::impl::sha3_256};
}

bool hash_type::impl::operator<(const hash_type::impl& rhs) const {
  return code() < rhs.code();
}
}  // namespace multihash
