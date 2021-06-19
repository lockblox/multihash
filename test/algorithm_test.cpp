#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <multihash/algorithm_registry.h>

#include "multihash_test.h"

using ::testing::StrEq;

struct test_parameters {
  multihash::algorithm_identifier_type algorithm_identifier;
  std::string digest;
};

namespace test {
class algorithm_test_fixture
    : public ::testing::TestWithParam<test_parameters> {
 protected:
  const multihash::algorithm_registry& algorithm_registry =
      multihash::algorithm_registry::default_instance();
  const std::string input{"foo"};
};

TEST_P(algorithm_test_fixture, digest) {
  auto algorithm =
      algorithm_registry.make_algorithm(GetParam().algorithm_identifier);
  auto digest = algorithm->digest(input.begin(), input.end());
  EXPECT_THAT(toHexString(digest), StrEq(GetParam().digest));
  digest = algorithm->digest(input.begin(), input.end());
  EXPECT_THAT(toHexString(digest), StrEq(GetParam().digest));
}

INSTANTIATE_TEST_CASE_P(
    algorithm_test, algorithm_test_fixture,
    ::testing::Values(
        test_parameters{multihash::algorithm_identifier::sha1,
                        "0beec7b5ea3f0fdbc95d0dd47f3c5bc275da8a33"},
        test_parameters{multihash::algorithm_identifier::sha2_256,
                        "2c26b46b68ffc68ff99b453c1d30413413422d706483bfa0f98a5e"
                        "886266e7ae"},
        test_parameters{
            multihash::algorithm_identifier::sha2_512,
            "f7fbba6e0636f890e56fbbf3283e524c6fa3204ae298382d624741d0dc6638326e"
            "282c41be5e4254d8820772c5518a2c5a8c0c7f7eda19594a7eb539453e1ed7"},
        test_parameters{multihash::algorithm_identifier::sha3_256,
                        "76d3bc41c9f588f7fcd0d5bf4718f8f84b1c41b20882703100b9eb"
                        "9413807c01"}));

TEST(algorithm_test, long_input) {
  auto source_chars =
      std::string("All work and no play makes Jack a dull boy.\n");
  auto repeat_count = 237;
  std::stringstream os;
  for (auto i = 0; i < repeat_count; ++i) {
    os << source_chars;
  }
  auto input = os.str();
  auto& algorithm_registry = multihash::algorithm_registry::default_instance();
  auto algorithm_identifier = multihash::algorithm_identifier::sha1;
  auto algorithm = algorithm_registry.make_algorithm(algorithm_identifier);
  auto digest = algorithm->digest(input.begin(), input.end());
  auto expected = "da96826ae365682f4123273c88e81479e899d8ac";
  EXPECT_THAT(toHexString(digest), StrEq(expected));
  digest = algorithm->digest(input.begin(), input.end());
  EXPECT_THAT(toHexString(digest), StrEq(expected));
}
}  // namespace test