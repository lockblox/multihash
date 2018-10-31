#pragma once

#ifdef WIN32
#include <string>

namespace multihash {
using string_view = std::string;
}

#else
#include <experimental/string_view>

namespace multihash {
// using string_view = std::experimental::fundamentals_v1::string_view;
using string_view = std::string;
}  // namespace multihash

#endif
