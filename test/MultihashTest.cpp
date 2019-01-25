#include <multihash/hash_function.h>
#include <multihash/hash_type.h>
#include <iomanip>
#include "gtest/gtest.h"

std::string toHexString(const multihash::multihash &hash) {
  std::ostringstream os;
  os << hash;
  return os.str();
}

TEST(Multihash, hashing) {
  /** Decoding a hashi from raw bytes */
  std::string input("foo");
  std::istringstream input_stream(input);
  {
    multihash::hash_function hash_function(multihash::hash_code::SHA1);
    auto hash = hash_function(input_stream);
    {
      auto expected = static_cast<unsigned char>(multihash::hash_code::SHA1);
      auto result = static_cast<unsigned char>(hash.code());
      EXPECT_EQ(expected, result);
    }
    {
      auto expected = "11140beec7b5ea3f0fdbc95d0dd47f3c5bc275da8a33";
      auto result = toHexString(hash);
      EXPECT_EQ(expected, result);
    }
  }
  {
    auto source_chars =
        std::string("All work and no play makes Jack a dull boy.\n");
    auto repeat_count = 1000000;
    std::stringstream os;
    for (auto i = 0; i < repeat_count; ++i) {
      os << source_chars;
    }
    auto input = os.str();

    multihash::hash_function hash_function(multihash::hash_code::SHA1);
    auto hash = hash_function(input);
    {
      auto expected = "11147dd3e2edbe26687c037094e7cd3d8f5c5e89e9ed";
      auto result = toHexString(hash);
      EXPECT_EQ(expected, result);

      // hash again
      hash = hash_function(input);
      result = toHexString(hash);
      EXPECT_EQ(expected, result);
    }
  }
  {
    input_stream.clear();
    input_stream.seekg(0);

    auto hash_function = multihash::hash_function(multihash::hash_code::SHA2_256);
    auto hash = hash_function(input_stream);
    {
      auto expected =
          static_cast<unsigned char>(multihash::hash_code::SHA2_256);
      auto result = static_cast<unsigned char>(hash.code());
      EXPECT_EQ(expected, result);
    }
    {
      auto expected =
          "12202c26b46b68ffc68ff99b453"
          "c1d30413413422d706483bfa0f98a5e886266e7ae";
      auto result = toHexString(hash);
      EXPECT_EQ(expected, result);

      // hash again
      hash = hash_function(input);
      result = toHexString(hash);
      EXPECT_EQ(expected, result);
    }
  }
  {
    input_stream.clear();
    input_stream.seekg(0);

    auto hash_function = multihash::hash_function(multihash::hash_code::SHA2_512);
    auto hash = hash_function(input_stream);
    {
      auto expected =
          static_cast<unsigned char>(multihash::hash_code::SHA2_512);
      auto result = static_cast<unsigned char>(hash.code());
      EXPECT_EQ(expected, result);
    }
    {
      auto expected =
          "1340f7fbba6e0636f890e56fbbf"
          "3283e524c6fa3204ae298382d624741d0dc6638326e282c41be5"
          "e4254d8820772c5518a2c5a8c0c7f7eda19594a7eb539453e1ed7";
      auto result = toHexString(hash);
      EXPECT_EQ(expected, result);
    }
  }
  {
    input_stream.clear();
    input_stream.seekg(0);
    auto hash_function = multihash::hash_function(multihash::hash_code::SHA3_256);
    auto hash_function2(hash_function);
    EXPECT_EQ(hash_function, hash_function2);
    auto hash = hash_function(input_stream);
    {
      auto expected =
          static_cast<unsigned char>(multihash::hash_code::SHA3_256);
      auto result = static_cast<unsigned char>(hash.code());
      EXPECT_EQ(expected, result);
    }
    {
      auto expected =
          "162076d3bc41c9f588f7fcd0d5bf4718f8f84b1c41b20882703100b9eb9413807c0"
          "1";
      auto result = toHexString(hash);
      EXPECT_EQ(expected, result);
    }
  }

  {
    auto hash_type = multihash::hash_type();
    EXPECT_THROW(multihash::hash_function{hash_type.code()}, std::out_of_range);
  }

  {
    auto hash_function = multihash::hash_function(multihash::hash_code::SHA2_512);
    auto hash_function_b = hash_function;
    EXPECT_EQ(hash_function, hash_function_b);
  }
}

TEST(Multihash, encoding) {
  std::istringstream input_stream("foo");
  multihash::hash_function hash_function(multihash::hash_code::SHA1);
  auto hash = hash_function(input_stream);
  auto encoded = static_cast<std::vector<char>>(hash);
  auto view = std::string_view(encoded.data(), encoded.size());
  auto decoded = multihash::multihash(view);
  EXPECT_EQ(hash, decoded);
}

TEST(Multihash, Default) {
  multihash::hash_function hash_function;
  EXPECT_EQ(multihash::hash_code::SHA2_256, hash_function.code());
}

TEST(Multihash, Inequality) {
  multihash::hash_function hash;
  auto foo_hash = hash("foo");
  auto bar_hash = hash("bar");
  EXPECT_NE(foo_hash, bar_hash);
  multihash::multihash mh{};
  EXPECT_NE(mh, foo_hash);
  EXPECT_EQ(mh, mh);
  EXPECT_EQ(foo_hash, foo_hash);
  bar_hash = foo_hash;
  EXPECT_EQ(foo_hash, bar_hash);
}

TEST(Multihash, HashConstruction) {
  multihash::hash_function hash_function;
  std::string foo("foo");
  auto hash = hash_function(foo);
  auto expected = std::string(
      "12202c26b46b68ffc68ff99b453c1d3041341"
      "3422d706483bfa0f98a5e886266e7ae");
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

  EXPECT_EQ(hash_function(foo), hash_function(foo));
  EXPECT_EQ(hash, hash_function(foo));
  EXPECT_EQ(expected, toHexString(hash_function(foo)));
}
