#include <multihash/hash_type.h>
#include <sstream>
#include "gtest/gtest.h"

TEST(hash_type, conversions) {
  /** Looking up a hash type and querying properties */
  {
    auto expected("sha1");
    auto result(multihash::hash_type("sha1").name());
    EXPECT_EQ(expected, result);
  }

  {
    auto expected = static_cast<unsigned char>(multihash::hash_code::SHA1);
    auto result =
        static_cast<unsigned char>(multihash::hash_type("sha1").code());
    EXPECT_EQ(expected, result);
  }

  {
    auto expected = static_cast<unsigned char>(multihash::hash_code::SHA1);
    auto hash_type = multihash::hash_type(multihash::hash_code::SHA1);
    auto result = static_cast<unsigned char>(hash_type.code());
    EXPECT_EQ(expected, result);
    std::ostringstream os;
    os << multihash::hash_code::SHA1;
    EXPECT_EQ("11", os.str());
  }

  {
    auto expected = static_cast<unsigned char>(multihash::hash_code::SHA1);
    auto hash_type = multihash::hash_type("sha1");
    auto result = static_cast<unsigned char>(hash_type.code());
    EXPECT_EQ(expected, result);
  }

  {
    auto result = multihash::hash_type("sha2-256").size();
    decltype(result) expected = 32;
    EXPECT_EQ(expected, result);
  }

  /** Failing to look up a hash type */
  { EXPECT_THROW(multihash::hash_type("unknown_hash"), std::invalid_argument); }
  {
    EXPECT_THROW(multihash::hash_type(static_cast<multihash::hash_code>(0x84)),
                 std::invalid_argument);
  }
}

TEST(hash_type, stream) {
  auto name = "sha2-256";
  auto hash_type = multihash::hash_type(name);
  std::ostringstream os;
  os << hash_type;
  EXPECT_EQ(name, os.str());
}

TEST(hash_type, list) {
  auto hash_type = multihash::hash_type(multihash::hash_code::SHA3_256);
  auto all_types = multihash::hash_type::types();
  EXPECT_EQ(1, all_types.count(hash_type));
}

TEST(hash_type, comparisons) {
  auto hash_type = multihash::hash_type();
  auto sha1 = multihash::hash_type(multihash::hash_code::SHA1);
  EXPECT_GT(sha1, hash_type);
  EXPECT_LT(hash_type, sha1);
  EXPECT_NE(hash_type, sha1);
  EXPECT_EQ(hash_type, hash_type);
}
