#pragma once

#include <multihash/hash_code.h>
#include <set>
#include <string>

namespace multihash {

/** Interface for a hash Type */
class hash_type {
 public:
  hash_type();
  explicit hash_type(const std::string& name);
  explicit hash_type(hash_code code);
  hash_type(const hash_type& rhs);

  const std::string& name() const;
  hash_code code() const;
  size_t size() const; /**< Default length of the hash type */

  bool operator==(const hash_type& rhs) const;
  bool operator!=(const hash_type& rhs) const;
  bool operator<(const hash_type& rhs) const;
  bool operator>(const hash_type& rhs) const;

  static std::set<hash_type> types();

 private:
  class impl;
  const impl* pImpl;
};

std::ostream& operator<<(std::ostream& os, const hash_type& hash_type);
}  // namespace multihash
