#pragma once

#include <multihash/hash_type.h>

namespace multihash {

class hash_type::impl {
 public:
  impl(hash_code code, std::string name, size_t size);
  ~impl() = default;
  const std::string& name() const { return name_; }
  hash_code code() const { return code_; }
  size_t size() const { return size_; }

  bool operator<(const impl& rhs) const;

  typedef std::set<const impl*> Set;
  static const Set all_types();

 private:
  const static impl identity;
  const static impl sha1;
  const static impl sha2_256;
  const static impl sha2_512;
  const static impl sha3_256;

  const hash_code code_;
  const std::string name_;
  const size_t size_;
};
}  // namespace multihash
