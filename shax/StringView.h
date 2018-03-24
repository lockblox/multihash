#pragma once

#ifdef WIN32
#include <string>
namespace shax
{
using string_view = std::string;
}
#else
#include <experimental/string_view>
namespace shax
{
// using string_view = std::experimental::fundamentals_v1::string_view;
using string_view = std::string;
}
#endif
