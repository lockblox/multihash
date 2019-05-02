#pragma once
#include <varint/varint.h>
#include <array>
#include <set>

namespace multihash {

template <typename Container>
using varint_type = varint::uleb128<Container>;
using varint_view = varint_type<std::string_view>;

template <std::size_t N>
using const_varint = const varint::array<varint::codecs::uleb128, N>;

namespace code {

using varint_view = varint_view;

/** Convert a code value into a string */
std::string to_string(varint_view value);

/** Convert a string into a code value */
varint_view from_string(const std::string& name);

/** Convert a varint into a code value */
template <typename T>
varint_view from_value(const T& value) {
  return static_cast<varint_view>(static_cast<std::string_view>(value));
}

extern const varint_view identity;
extern const varint_view sha1;
extern const varint_view sha2_256;
extern const varint_view sha2_512;
extern const varint_view sha3_256;

std::set<std::string> names();
}  // namespace code

}  // namespace multihash