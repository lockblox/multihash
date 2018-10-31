#pragma once

#include <multihash/HashFunction.h>
#include <multihash/detail/Algorithm.h>

namespace multihash {

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
}  // namespace multihash
