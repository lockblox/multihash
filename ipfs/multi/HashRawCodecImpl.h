#pragma once

#include <ipfs/multi/HashRawCodec.h>

namespace ipfs {
namespace multi {

class HashRawCodec::Impl {
 public:
  std::vector<char> encode(const Hash& hash) const;
  Hash decode(const std::vector<char>& raw_bytes) const;
  static Impl* instance();

 private:
  static Impl instance_;
};
}  // namespace multi
}  // namespace ipfs
