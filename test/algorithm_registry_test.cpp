#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <multihash/algorithm_registry.h>

#include <sstream>

using ::testing::Eq;

class algorithm_registry_test : public ::testing::Test {
 protected:
  const auto& registry() const {
    return multihash::algorithm_registry::default_instance();
  }
};

TEST_F(algorithm_registry_test, find_by_name) {
  auto identifier = multihash::algorithm_identifier::sha1;
  auto factory = registry().at("sha1");
  EXPECT_THAT(factory.identifier(), Eq(identifier));
  EXPECT_THAT(factory.name(), Eq("sha1"));
}

TEST_F(algorithm_registry_test, find_by_id) {
  auto sha1 = multihash::algorithm_identifier::sha1;
  auto factory = registry().at(sha1);
  EXPECT_THAT(factory.identifier(), Eq(sha1));
  EXPECT_THAT(factory.name(), Eq("sha1"));
}

TEST_F(algorithm_registry_test, find_by_unknown_id) {
  auto factory = multihash::algorithm_factory{};
  EXPECT_THROW(factory=registry().at(23), std::out_of_range);
}

TEST_F(algorithm_registry_test, find_by_unknown_name) {
  auto factory = multihash::algorithm_factory{};
  EXPECT_THROW(factory=registry().at("sha1024"), std::out_of_range);
}