#pragma once
#include <multihash/digest.h>
#include <multihash/multihash.h>
#include <cassert>

namespace multihash {

class function {
 public:
  explicit function(code_type code = code::sha2_256);

  /** Compute the hash of input */
  template <typename Input>
  multihash<std::string> operator()(Input& input);

  /** Write the hash of input into a buffer */
  template <typename Input>
  multihash<string_span> operator()(Input& input, string_span output);

  /** Returns the code of this hash */
  code_type code() const;

  /** Returns the size of the multihash */
  std::size_t size();

 private:
  code_type code_;
  digest digest_;
};

template <typename Input>
multihash<std::string> function::operator()(Input& input) {
  auto output = std::string(size(), 0);
  auto span = string_span(output);
  (*this)(input, span);
  return multihash(output);
}

template <typename Input>
multihash<string_span> function::operator()(Input& input, string_span output) {
  auto digest_size = digest_.size();
  assert(this->size() <= std::size_t(output.size()));
  auto length = ::multihash::write(output, code(), digest_size);
  auto span = string_span(&output[length], digest_size);
  digest_(input, span);
  auto view = string_view(&output[length], digest_size);
  return multihash<string_span>(code(), view, output);
}

}  // namespace multihash
