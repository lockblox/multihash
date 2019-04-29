#pragma once
#include <varint/varint.h>
#include <array>
#include <set>

namespace multihash {

namespace code {

using uleb128 = varint::codecs::uleb128;

using value_type = varint::uleb128<std::string_view>;

/** Convert a code value into a string */
std::string to_string(value_type value);

/** Convert a string into a code value */
value_type from_string(const std::string& name);

namespace literals {
const auto identity = varint::array<uleb128, 0>{};
const auto sha1 = varint::array<uleb128, 0x11>{};
const auto sha2_256 = varint::array<uleb128, 0x12>{};
const auto sha2_512 = varint::array<uleb128, 0x13>{};
const auto sha3_256 = varint::array<uleb128, 0x16>{};
}  // namespace literals

const auto identity =
    static_cast<value_type>(static_cast<std::string_view>(literals::identity));
const auto sha1 =
    static_cast<value_type>(static_cast<std::string_view>(literals::sha1));
const auto sha2_256 =
    static_cast<value_type>(static_cast<std::string_view>(literals::sha2_256));
const auto sha2_512 =
    static_cast<value_type>(static_cast<std::string_view>(literals::sha2_512));
const auto sha3_256 =
    static_cast<value_type>(static_cast<std::string_view>(literals::sha3_256));

std::set<std::string> names();
}  // namespace code

using code_type = code::value_type;

}  // namespace multihash