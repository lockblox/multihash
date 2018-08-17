#include "HashRawCodecImpl.h"
#include <iomanip>
#include <sstream>

namespace multihash {

Hash HashRawCodec::Impl::decode(const std::vector<char> &raw_bytes) const {
  auto code = static_cast<HashCode>((raw_bytes.at(0)));
  auto size = uint8_t(raw_bytes.at(1));
  auto hash_type = HashType(code);

  std::vector<char> digest(begin(raw_bytes) + 2, end(raw_bytes));
  if (digest.size() != size) {
    std::ostringstream err;
    err << "Header misreports digest size as " << static_cast<int>(size)
        << "; true size is " << digest.size();
    throw std::range_error(err.str());
  }
  if (size != hash_type.size()) {
    std::ostringstream err;
    err << "Non standard digest size " << static_cast<int>(size)
        << "; expected " << hash_type.size();
    throw std::range_error(err.str());
  }
  return Hash(std::move(hash_type), digest);
}

HashRawCodec::Impl *HashRawCodec::Impl::instance() { return &instance_; }

std::vector<char> HashRawCodec::Impl::encode(const Hash &hash) const {
  auto size = uint8_t(hash.digest().size());
  auto code = uint8_t(hash.type().code());
  std::vector<char> data;
  data.reserve(size + 2);
  data.push_back(code);
  data.push_back(size);
  std::copy(begin(hash.digest()), end(hash.digest()), std::back_inserter(data));
  return data;
}

HashRawCodec::Impl HashRawCodec::Impl::instance_ = HashRawCodec::Impl();

}  // namespace multihash
