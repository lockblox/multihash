#pragma once

#include <cstddef>
#include <gsl/string_span>
#include <map>
#include <memory>
#include <string_view>

namespace multihash {

using string_view = std::string_view;
using string_span = gsl::span<char>;

class algorithm {
 public:
  virtual ~algorithm() = default;
  virtual std::string name() const = 0;
  virtual void reset() = 0;
  virtual std::size_t digest_size() const = 0;
  virtual std::size_t block_size() const = 0;
  virtual void update(string_view data) = 0;
  virtual std::size_t digest(string_span output) = 0;

  /** Unique identifier for algorithms */
  using code_type = char;

  /** Identifiers for built-in algorithms */
  enum code : code_type {
    identity = 0x00,
    sha1 = 0x11,
    sha2_256 = 0x12,
    sha2_512 = 0x13,
    sha3_256 = 0x16
  };

  using codename = std::pair<code, std::string>;

  class factory {
   public:
    virtual ~factory() = default;
    virtual std::unique_ptr<algorithm> create() = 0;
  };

  static void register_factory(algorithm::codename codename, factory& factory);
  static std::unique_ptr<algorithm> create(algorithm::code_type code);

  /** Determine the code of a given name */
  static code_type code(std::string name);

  /** Determine the name of a given code */
  static std::string name(code_type code);

  static void register_codename(algorithm::codename value);
  static const std::map<code_type, std::string>& codenames();

 private:
  static std::map<code_type, std::string> codenames_;
  static std::map<algorithm::code_type, factory*> factories_;
};

}  // namespace multihash
