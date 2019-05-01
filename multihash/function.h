#pragma once
#include <multihash/digest.h>
#include <multihash/multihash.h>
#include <cassert>

namespace multihash {

class function {
 public:
  explicit function(code_type code = code::sha2_256);

  /** Compute the hash of an input range */
  template <typename InputIterator>
  multihash<std::string> operator()(InputIterator first, InputIterator last);

  /** Write the hash of an input range into output */
  template <typename InputIterator, typename OutputIterator>
  OutputIterator operator()(InputIterator first, InputIterator last,
                            OutputIterator output);

  /** Returns the code of this hash */
  code_type code() const;

  /** Returns the size of the multihash */
  std::size_t size();

 private:
  code_type code_;
  digest digest_;
};

template <typename InputIterator>
multihash<std::string> function::operator()(InputIterator first,
                                            InputIterator last) {
  return multihash<std::string>{code(), digest_(first, last)};
}

template <typename InputIterator, typename OutputIterator>
OutputIterator function::operator()(InputIterator first, InputIterator last,
                                    OutputIterator output) {
  auto code_view = static_cast<std::string_view>(code());
  output = std::copy(code_view.begin(), code_view.end(), output);
  output++ = size();
  digest_(first, last, output);
  return output;
}

}  // namespace multihash
