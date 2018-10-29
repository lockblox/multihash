#pragma once

#include <ipfs/multi/HashFunction.h>
#include <ipfs/multi/detail/Algorithm.h>

namespace ipfs {
namespace multi {

class HashFunction::Impl {
 public:
  explicit Impl(HashType hash_type);
  ~Impl() = default;
  const HashType& type() const { return hash_type_; }
  Hash operator()(std::istream& input);
  Hash operator()(string_view input);

 private:
  void init();
  HashType hash_type_;
  std::unique_ptr<Algorithm> algorithm_;
};
}  // namespace multi
}  // namespace ipfs
