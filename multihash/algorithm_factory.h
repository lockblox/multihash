#pragma once

#include <multihash/algorithm.h>
#include <multihash/algorithm_identifier.h>

#include <functional>
#include <memory>

namespace multihash {

/** Creates uniquely identified algorithm instances */
class algorithm_factory {
 public:
  /** Function used to create an instance */
  using function_type = std::function<std::unique_ptr<algorithm>(void)>;

  /** Creates a sentinel uninitialised factory */
  algorithm_factory() = default;

  /** Creates a uniquely identified algorithm factory */
  algorithm_factory(algorithm_identifier_type identifier, std::string name,
                    function_type function);

  /** Returns the numeric algorithm identifier */
  [[nodiscard]] algorithm_identifier_type identifier() const;
  /** Returns the textual algorithm identifier */
  [[nodiscard]] std::string_view name() const;
  /** Creates an instance of the algorithm */
  [[nodiscard]] std::unique_ptr<algorithm> make_algorithm() const;

 private:
  algorithm_identifier_type identifier_ = 0;
  std::string name_;
  function_type function_;
};

bool operator<(const algorithm_factory& lhs, const algorithm_factory& rhs);

}  // namespace multihash