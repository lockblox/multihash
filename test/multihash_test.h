#pragma once
#include <multihash/multihash.h>

#include <iomanip>

namespace test {

template <typename Container>
std::ostream& printToStream(std::ostream& os,
                         const Container& hash) {
  for (auto c : hash) {
    auto uc = uint8_t(c);
    os << std::hex;
    os << std::setfill('0');
    os << std::setw(2);
    os << static_cast<int>(uc);
  }
  return os;
}

template <typename Container>
std::string toHexString(const Container& hash) {
  std::ostringstream os;
  printToStream(os, hash);
  return os.str();
}

}  // namespace