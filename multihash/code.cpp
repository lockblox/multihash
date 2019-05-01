#include <multihash/code.h>
#include <algorithm>
#include <iterator>
#include <map>

namespace multihash {

namespace code {

const value_type identity = from_value(literals::identity);
const value_type sha1 = from_value(literals::sha1);
const value_type sha2_256 = from_value(literals::sha2_256);
const value_type sha2_512 = from_value(literals::sha2_512);
const value_type sha3_256 = from_value(literals::sha3_256);

std::map<code_type, std::string, std::less<>> codenames{{sha1, "sha1"},
                                                        {sha2_256, "sha2-256"},
                                                        {sha2_512, "sha2-512"},
                                                        {sha3_256, "sha3-256"}};

value_type from_string(const std::string& name) {
  auto it = std::find_if(codenames.begin(), codenames.end(),
                         [&name](auto i) { return i.second == name; });
  if (it == codenames.end()) {
    throw std::out_of_range("Unknown code " + name);
  }
  return it->first;
}

std::string to_string(value_type code) { return codenames.at(code); }

std::set<std::string> names() {
  auto result = std::set<std::string>{};
  std::transform(codenames.begin(), codenames.end(),
                 std::inserter(result, result.end()),
                 [](auto& i) { return i.second; });
  return result;
}

}  // namespace code
}  // namespace multihash