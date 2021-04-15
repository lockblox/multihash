#include <multihash/algorithm_factory.h>

namespace multihash {

algorithm_factory::algorithm_factory(algorithm_identifier identifier,
                                     std::string name,
                                     algorithm_factory::function_type function)
    : algorithm_factory(static_cast<algorithm_identifier_type>(identifier),
                        std::move(name), std::move(function)) {}

algorithm_factory::algorithm_factory(algorithm_identifier_type identifier,
                                     std::string name,
                                     algorithm_factory::function_type function)
    : identifier_(identifier),
      name_(std::move(name)),
      function_(std::move(function)) {}

algorithm_identifier_type algorithm_factory::identifier() const {
  return identifier_;
}

std::string algorithm_factory::name() const { return name_; }

std::unique_ptr<algorithm> algorithm_factory::create() const {
  return function_();
}

}  // namespace multihash