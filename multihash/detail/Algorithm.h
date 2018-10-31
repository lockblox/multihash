#pragma once

#include <cryptlib.h>
#include <multihash/StringView.h>
#include <vector>

namespace multihash {

struct Algorithm {
  virtual size_t block_size() = 0;
  virtual void update(const string_view& data) = 0;
  virtual std::vector<char> digest() = 0;
  virtual ~Algorithm() = default;
};
}  // namespace multihash
