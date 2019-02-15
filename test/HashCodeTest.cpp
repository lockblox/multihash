#include <gtest/gtest.h>
#include <multihash/function.h>
#include <multihash/static_function.h>
#include <sstream>

TEST(hash_type, conversions) {
  /** Looking up a hash type and querying properties */
  {
    auto expected("sha-1");
    auto code = multihash::algorithm::sha1;
    auto result(multihash::algorithm::create(code)->name());
    EXPECT_EQ(expected, result);
  }
  {
    auto expected = static_cast<unsigned char>(multihash::algorithm::sha1);
    auto code = multihash::algorithm::code("sha1");
    auto algo = multihash::algorithm::create(code);
    ASSERT_TRUE(algo);
    EXPECT_EQ(expected, code);
  }
  {
    auto expected = static_cast<unsigned char>(multihash::algorithm::sha1);
    auto hash_type = multihash::function(multihash::algorithm::sha1);
    auto result = static_cast<unsigned char>(hash_type.code());
    EXPECT_EQ(expected, result);
  }

  {
    auto expected = static_cast<unsigned char>(multihash::algorithm::sha1);
    auto code = multihash::algorithm::code("sha1");
    EXPECT_EQ(expected, code);
  }

  {
    auto result = multihash::sha2_256{}.size();
    decltype(result) expected = 32;
    EXPECT_EQ(expected, result);
  }

  // Failing to look up a hash type
  {
    EXPECT_THROW(multihash::algorithm::code("unknown_hash"), std::out_of_range);
    EXPECT_THROW(multihash::algorithm::name((0x84)), std::out_of_range);
  }
}
