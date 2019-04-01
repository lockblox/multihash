#include "digest.h"
#include <multihash/digest.h>

namespace multihash {

digest::digest(std::unique_ptr<algorithm> algorithm)
    : algorithm_(std::move(algorithm)) {}

std::string digest::operator()(std::istream& input) {
  auto output = std::string(algorithm_->digest_size(), 0);
  auto span = string_span(output);
  (*this)(input, span);
  return output;
}

std::string digest::operator()(string_view input) {
  auto output = std::string(algorithm_->digest_size(), 0);
  auto span = string_span(output);
  (*this)(input, span);
  return output;
}

std::size_t digest::operator()(std::istream& input, string_span output) {
  if (!input.good()) {
    throw std::invalid_argument("hash input is not good");
  }
  auto buffer = std::string(algorithm_->block_size(), ' ');
  auto begin = buffer.begin();
  auto end = buffer.end();

  while (!input.eof()) {
    input.read(const_cast<char*>(buffer.data()), buffer.size());

    if (!input.eof()) {
      // filled the buffer so update the hash
      algorithm_->update(buffer);
    }
  }
  // final update to hash with partially filled vector
  auto chars_read = int(input.gcount());
  std::advance(begin, chars_read);
  buffer.erase(begin, end);
  if (!buffer.empty()) {
    algorithm_->update(buffer);
  }
  return algorithm_->digest(output);
}

std::size_t digest::operator()(string_view input, string_span output) {
  algorithm_->reset();
  auto block_size = algorithm_->block_size();
  auto begin = input.begin();
  auto size = std::min(input.size(), block_size);
  auto remaining = input.size();
  auto end = begin;
  while (size > 0) {
    std::advance(end, size);
    auto buffer = std::string_view(&(*begin), size);
    algorithm_->update(buffer);
    begin = end;
    remaining -= size;
    size = std::min(remaining, block_size);
  }
  return algorithm_->digest(output);
}

bool digest::verify(string_view input, string_view hash) {
  auto verified_hash = (*this)(input);
  auto length = std::min(verified_hash.length(), hash.length());
  return std::equal(hash.begin(), hash.begin() + length, verified_hash.begin(),
                    verified_hash.end());
}

std::size_t digest::size() { return algorithm_->digest_size(); }

}  // namespace multihash
