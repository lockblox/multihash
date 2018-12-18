#pragma once

#include <cryptlib.h>
#include <vector>
#include <string_view>

namespace multihash {

struct algorithm {
  virtual size_t block_size() = 0;
  virtual void update(const std::string_view& data) = 0;
  virtual std::vector<char> digest() = 0;
  virtual ~algorithm() = default;
};
}  // namespace multihash
