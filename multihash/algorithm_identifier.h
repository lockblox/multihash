#pragma once
#include <type_traits>

namespace multihash {

enum class algorithm_identifier {
  identity = 0,
  sha1 = 0x11,
  sha2_256 = 0x12,
  sha2_512 = 0x13,
  sha3_256 = 0x16
};

using algorithm_identifier_type = std::underlying_type<algorithm_identifier>;

}  // namespace multihash