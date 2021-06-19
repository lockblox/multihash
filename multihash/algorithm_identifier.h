#pragma once
#include <type_traits>

namespace multihash {

using algorithm_identifier_type = unsigned int;

namespace algorithm_identifier {
constexpr algorithm_identifier_type identity = 0;
constexpr algorithm_identifier_type sha1 = 0x11;
constexpr algorithm_identifier_type sha2_256 = 0x12;
constexpr algorithm_identifier_type sha2_512 = 0x13;
constexpr algorithm_identifier_type sha3_256 = 0x16;

}  // namespace algorithm_identifier

}  // namespace multihash