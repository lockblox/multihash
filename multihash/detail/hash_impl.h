#pragma once

#include <multihash/detail/algorithm.h>
#include <multihash/hash.h>

namespace multihash {

class hash::impl {
 public:
  explicit impl(hash_code hash_code);
  ~impl() = default;
  const hash_code& code() const { return hash_code_; }
  multihash operator()(std::istream& input);
  multihash operator()(std::string_view input);

 private:
  void init();
  multihash digest();
  hash_code hash_code_;
  std::unique_ptr<algorithm> algorithm_;
};
}  // namespace multihash
