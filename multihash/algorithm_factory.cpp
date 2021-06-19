#include <multihash/algorithm_factory.h>

namespace multihash {

algorithm_factory::algorithm_factory(algorithm_identifier_type identifier,
                                     std::string name,
                                     algorithm_factory::function_type function)
    : identifier_(identifier),
      name_(std::move(name)),
      function_(std::move(function)) {}

algorithm_identifier_type algorithm_factory::identifier() const {
  return identifier_;
}

std::string_view algorithm_factory::name() const { return name_; }

std::unique_ptr<algorithm> algorithm_factory::make_algorithm() const {
  return function_();
}

bool operator<(const algorithm_factory& lhs, const algorithm_factory& rhs) {
  bool result = false;
  if (lhs.identifier() < rhs.identifier())
  {
    result = true;
  }
  else if (lhs.identifier() == rhs.identifier() && lhs.name() < rhs.name()) {
    result = true;
  }
  return result;
}

}  // namespace multihash