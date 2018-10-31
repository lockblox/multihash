#pragma once

#include <multihash/HashRawCodec.h>

namespace multihash {

class HashRawCodec::Impl {
 public:
  std::vector<char> encode(const Hash& hash) const;
  Hash decode(const std::vector<char>& raw_bytes) const;
  static Impl* instance();

 private:
  static Impl instance_;
};
}  // namespace multihash
