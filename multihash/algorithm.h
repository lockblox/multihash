#pragma once

#include <multihash/code.h>
#include <cstddef>
#include <gsl/string_span>
#include <map>
#include <memory>
#include <string_view>

namespace multihash {

using string_span = gsl::span<char>;

class algorithm {
 public:
  virtual ~algorithm() = default;
  virtual void reset() = 0;
  virtual std::size_t digest_size() const = 0;
  virtual std::size_t block_size() const = 0;
  virtual void update(std::string_view data) = 0;
  virtual std::size_t digest(string_span output) = 0;
  std::string digest();

  class factory {
   public:
    virtual ~factory() = default;
    virtual std::unique_ptr<algorithm> create() = 0;
  };

  static std::unique_ptr<algorithm> create(code_type code);

 private:
  static std::map<code_type, factory*> factories();
};

}  // namespace multihash