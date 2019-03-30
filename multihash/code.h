#pragma once
#include <set>

namespace multihash {

namespace code {

using value_type = char;

/** Convert a code value into a string */
std::string to_string(value_type value);

/** Convert a string into a code value */
value_type from_string(const std::string& name);

constexpr value_type identity = 0x00;
constexpr value_type sha1 = 0x11;
constexpr value_type sha2_256 = 0x12;
constexpr value_type sha2_512 = 0x13;
constexpr value_type sha3_256 = 0x16;

std::set<std::string> names();
}  // namespace code

using code_type = code::value_type;

}  // namespace multihash