#pragma once

#include <multihash/algorithm.h>
#include <multihash/algorithm_factory.h>
#include <functional>
#include <memory>

namespace multihash {

class algorithm_registry {
 public:
  /** Returns the default algorithm registry */
  static const algorithm_registry& default_instance();

  algorithm_registry& register_algorithm(algorithm_factory factory);

  using container_type = std::set<algorithm_factory>;
  using iterator = container_type::const_iterator;
  using const_iterator = container_type::const_iterator;

  /** Returns an iterator pointing to the first registered algorithm factory */
  const_iterator begin() const;

  /** Returns an iterator pointing to the element after the last
   * registered algorithm factory */
  const_iterator end() const;

  algorithm_identifier identifier(const std::string& name);
  const std::string& name(algorithm_identifier identifier);

  std::unique_ptr<algorithm> create(const std::string& name) const;
  std::unique_ptr<algorithm> create(algorithm_identifier_type id) const;

 private:
  std::set<algorithm_factory> algorithm_factories_;
};

}  // namespace multihash