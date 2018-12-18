#pragma once
#include <ostream>

namespace multihash {

/** Code values of supported hash types */
enum class hash_code : char {
  IDENTITY = 0x00,
  SHA1 = 0x11,      //"sha1"
  SHA2_256 = 0x12,  //"sha2-256"
  SHA2_512 = 0x13,  //"sha2-512"
  SHA3 = 0x14,      //"sha3"
};

std::ostream& operator<<(std::ostream& os, hash_code code);

}  // namespace multihash
