#include "hash_code.h"

namespace multihash {

std::ostream& operator<<(std::ostream& os, hash_code code) {
  os << std::hex << static_cast<int>(code);
  return os;
}

}  // namespace multihash
