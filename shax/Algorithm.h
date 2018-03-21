#pragma once

#include <experimental/string_view>
#include <vector>

namespace shax
{

using string_view = std::experimental::fundamentals_v1::string_view;

struct Algorithm
{
    virtual size_t block_size() = 0;
    virtual void init() = 0;
    virtual void update(const string_view& data) = 0;
    virtual std::vector<char> digest() = 0;
    virtual ~Algorithm() = default;
};

} // namespace shax
