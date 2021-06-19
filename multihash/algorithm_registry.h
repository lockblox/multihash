#pragma once

#include <multihash/algorithm.h>
#include <multihash/algorithm_factory.h>

#include <functional>
#include <memory>
#include <vector>

namespace multihash {

class algorithm_registry {
 public:
  /** Returns the default algorithm registry */
  static const algorithm_registry& default_instance();

  algorithm_registry& register_algorithm(const algorithm_factory& factory);

  using container_type = std::vector<algorithm_factory>;
  using const_iterator = container_type::const_iterator;

  /** Returns an iterator pointing to the first registered algorithm factory */
  const_iterator begin() const;

  /** Returns an iterator pointing to the element after the last
   * registered algorithm factory */
  const_iterator end() const;

  const_iterator find(const std::string& algorithm_name) const;
  const_iterator find(algorithm_identifier_type algorithm_identifier) const;

  [[nodiscard]] const algorithm_factory& at(
      const std::string& algorithm_name) const;
  [[nodiscard]] const algorithm_factory& at(
      algorithm_identifier_type algorithm_identifier) const;

  [[nodiscard]] std::unique_ptr<algorithm> make_algorithm(
      const std::string& algorithm_name) const;

  [[nodiscard]] std::unique_ptr<algorithm> make_algorithm(
      algorithm_identifier_type algorithm_identifier) const;

 private:
  container_type algorithm_factories_;
};

}  // namespace multihash