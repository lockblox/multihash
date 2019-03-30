#include <multihash/code.h>
#include <algorithm>
#include <iterator>
#include <map>

namespace {

std::map<multihash::code::value_type, std::string> codenames{
    {multihash::code::sha1, "sha1"},
    {multihash::code::sha2_256, "sha2-256"},
    {multihash::code::sha2_512, "sha2-512"},
    {multihash::code::sha3_256, "sha3-256"}};
}

namespace multihash {
namespace code {

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