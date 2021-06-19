#include "multihash_test.h"

#include <gtest/gtest.h>
#include <multihash/algorithm_registry.h>
#include <multihash/multihash.h>

using namespace std::string_literals;

namespace test {

class multihash_test : public ::testing::Test {
 protected:
  using multihash_string = multihash::multihash<std::string>;

  const multihash::algorithm_registry& algorithm_registry =
      multihash::algorithm_registry::default_instance();

  std::unique_ptr<multihash::algorithm> algorithm =
      algorithm_registry.make_algorithm(
          multihash::algorithm_identifier::sha2_256);

  varint::array<varint::codecs::uleb128,
                multihash::algorithm_identifier::sha2_256>
      algorithm_identifier;

  multihash::code_type code{
      static_cast<std::string_view>(algorithm_identifier)};
};

TEST_F(multihash_test, encoding) {
  std::istringstream input_stream("foo");
  auto hash = algorithm->digest(std::istream_iterator<char>(input_stream),
                                std::istream_iterator<char>());
}

TEST_F(multihash_test, inequality) {
  std::string foo = "foo";
  auto foo_hash = multihash_string{algorithm->digest(foo.begin(), foo.end())};
  std::string bar = "bar";
  auto bar_hash = multihash_string{algorithm->digest(bar.begin(), bar.end())};
  EXPECT_NE(foo_hash, bar_hash);
  multihash::multihash mh{std::string{"blah"}};
  EXPECT_NE(mh, foo_hash);
  EXPECT_EQ(mh, mh);
  EXPECT_EQ(foo_hash, foo_hash);
  bar_hash = foo_hash;
  EXPECT_EQ(foo_hash, bar_hash);
}

TEST_F(multihash_test, hash_construction) {
  std::string foo("foo");
  auto hash = multihash_string{code, algorithm->digest(foo.begin(), foo.end())};
  auto expected = std::string(
      "12202c26b46b68ffc68ff99b453c1d30413413422d706483bfa0f98a5e886266e7ae");
  EXPECT_EQ(expected, toHexString(hash));
  {  // copy assignment
    auto hash_copied = hash;
    EXPECT_EQ(expected, toHexString(hash_copied));
    EXPECT_EQ(expected, toHexString(hash));
    EXPECT_EQ(hash_copied, hash);
  }
  {  // copy construction
    multihash::multihash hash_copied(hash);
    EXPECT_EQ(expected, toHexString(hash));
    EXPECT_EQ(hash_copied, hash);
  }

  EXPECT_EQ(
      (multihash_string{code, algorithm->digest(foo.begin(), foo.end())}),
      (multihash_string{code, algorithm->digest(foo.begin(), foo.end())}));
  EXPECT_EQ(hash, (multihash_string{
                      code, algorithm->digest(foo.begin(), foo.end())}));
  EXPECT_EQ(expected, toHexString(multihash_string{
                          code, algorithm->digest(foo.begin(), foo.end())}));

  auto expected_view = std::string_view(expected);
  auto buffer = std::string{};
  buffer.resize(expected.size() + 2);
  auto view = multihash::string_span(buffer);
  {
    multihash::multihash<multihash::string_span> h(code, expected_view, view);
    EXPECT_EQ(code, h.code());
    EXPECT_EQ(expected.size(), h.digest().size());
    EXPECT_EQ(expected, h.digest());
  }
  {
    multihash::multihash<std::string> h(code, expected_view);
    EXPECT_EQ(code, h.code());
    EXPECT_EQ(expected.size(), h.digest().size());
    EXPECT_EQ(expected, h.digest());
  }
}

TEST_F(multihash_test, digest) {
  multihash::multihash digest{};
  EXPECT_TRUE(digest.empty());
}

}  // namespace test