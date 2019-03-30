#include <gtest/gtest.h>
#include <multihash/function.h>
#include <multihash/static_function.h>
#include <sstream>

TEST(hash_type, conversions) {
  {
    auto expected = static_cast<unsigned char>(multihash::code::sha1);
    auto code = multihash::code::from_string("sha1");
    auto algo = multihash::algorithm::create(code);
    ASSERT_TRUE(algo);
    EXPECT_EQ(expected, code);
  }
  {
    auto expected = static_cast<unsigned char>(multihash::code::sha1);
    auto hash_type = multihash::function(multihash::code::sha1);
    auto result = static_cast<unsigned char>(hash_type.code());
    EXPECT_EQ(expected, result);
  }

  {
    auto expected = static_cast<unsigned char>(multihash::code::sha1);
    auto code = multihash::code::from_string("sha1");
    EXPECT_EQ(expected, code);
  }

  {
    auto result = multihash::sha2_256{}.size();
    decltype(result) expected = 32;
    EXPECT_EQ(expected, result);
  }

  // Failing to look up a hash type
  {
    auto string_input = std::string("unknown hash");
    EXPECT_THROW(multihash::code::from_string(string_input), std::out_of_range);
    auto value_input = multihash::code_type(127);
    EXPECT_THROW(multihash::code::to_string(value_input), std::out_of_range);
  }
}
