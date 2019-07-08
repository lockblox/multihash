#pragma once
#include <varint/varint.h>
#include <array>
#include <set>

namespace multihash {

template <typename Container>
using varint_type = varint::uleb128<Container>;
using code_type = varint_type<std::string_view>;

template <std::size_t N>
using const_varint = const varint::array<varint::codecs::uleb128, N>;

namespace code {

/** Convert a code value into a string */
std::string to_string(code_type value);

/** Convert a string into a code value */
code_type from_string(const std::string& name);

/** Convert a varint into a code value */
template <typename T>
code_type from_value(const T& value) {
  return static_cast<code_type>(static_cast<std::string_view>(value));
}

extern const code_type identity;
extern const code_type sha1;
extern const code_type sha2_256;
extern const code_type sha2_512;
extern const code_type sha3_256;

std::set<std::string> names();
}  // namespace code

}  // namespace multihash