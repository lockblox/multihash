#include <multihash/multihash.h>

namespace multihash {

std::size_t write(string_span output, code_type code, std::size_t digest_size) {
  assert(std::size_t(output.size()) >= size(code, digest_size));
  auto length = sizeof(code) + 1;
  auto it = output.begin();
  std::copy(&code, &code + sizeof(code), it);
  it += sizeof(code);
  *it = char(digest_size);
  return length;
}

std::size_t write(string_span output, code_type code, string_view digest) {
  assert(std::size_t(output.size()) >= size(code, digest));
  auto size = write(output, code, digest.size());
  auto it = output.begin() + size;
  std::copy(digest.begin(), digest.end(), it);
  size += digest.size();
  return size;
}

}  // namespace multihash