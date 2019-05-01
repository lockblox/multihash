#pragma once
#include <varint/varint.h>
#include <array>
#include <set>

namespace multihash {

namespace code {

template <std::size_t N>
using varint_type = const varint::array<varint::codecs::uleb128, N>;

using value_type = varint::uleb128<std::string_view>;

/** Convert a code value into a string */
std::string to_string(value_type value);

/** Convert a string into a code value */
value_type from_string(const std::string& name);

/** Convert a varint into a code value */
template <typename Container>
constexpr value_type from_value(const varint::uleb128<Container>& value) {
  return static_cast<value_type>(static_cast<std::string_view>(value));
}

namespace literals {
const auto identity = varint_type<0>{};
const auto sha1 = varint_type<0x11>{};
const auto sha2_256 = varint_type<0x12>{};
const auto sha2_512 = varint_type<0x13>{};
const auto sha3_256 = varint_type<0x16>{};
}  // namespace literals

extern const value_type identity;
extern const value_type sha1;
extern const value_type sha2_256;
extern const value_type sha2_512;
extern const value_type sha3_256;

std::set<std::string> names();
}  // namespace code

using code_type = code::value_type;

}  // namespace multihash