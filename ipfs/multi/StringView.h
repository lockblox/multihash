#pragma once

#ifdef WIN32
#include <string>
namespace ipfs {
namespace multi {
using string_view = std::string;
}
}  // namespace ipfs
#else
#include <experimental/string_view>

namespace ipfs {
namespace multi {
// using string_view = std::experimental::fundamentals_v1::string_view;
using string_view = std::string;
}  // namespace multi
}  // namespace ipfs
#endif
