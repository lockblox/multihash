#include <gtest/gtest.h>
#include <multihash/function.h>
#include <multihash/static_function.h>
#include <sstream>

TEST(multihash, hash_type) {
  {
    auto expected = multihash::code::sha1;
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
    auto expected = multihash::code::sha1;
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
    auto code_buffer = std::vector<char>{127u};
    auto code_view = std::string_view(&code_buffer[0], code_buffer.size());
    auto value_input = multihash::varint_view(code_view);
    EXPECT_THROW(multihash::code::to_string(value_input), std::out_of_range);
  }
}
