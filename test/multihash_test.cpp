#include <gtest/gtest.h>
#include <multihash/digest.h>
#include <multihash/function.h>
#include <iomanip>

using namespace std::string_literals;

namespace {

template <typename Container>
std::ostream& operator<<(std::ostream& os,
                         const multihash::digest<Container>& hash) {
  auto data = std::string_view(hash.data(), hash.size());
  for (auto c : data) {
    auto uc = uint8_t(c);
    os << std::hex << std::setfill('0') << std::setw(2);
    os << static_cast<int>(uc);
  }
  return os;
}

template <typename Container>
std::string toHexString(const multihash::digest<Container>& hash) {
  std::ostringstream os;
  os << hash;
  return os.str();
}

}  // namespace

TEST(multihash, hashing) {
  {
    std::string input("foo");
    std::istringstream input_stream(input);
    {
      auto hash = multihash::function(multihash::code::sha1)(input.begin(),
                                                             input.end());
      {
        auto expected = static_cast<unsigned char>(multihash::code::sha1);
        auto result = static_cast<unsigned char>(hash.code());
        EXPECT_EQ(expected, result);
      }
      {
        auto expected = "11140beec7b5ea3f0fdbc95d0dd47f3c5bc275da8a33";
        auto result = toHexString(hash);
        EXPECT_EQ(expected, result);
      }
    }
    input_stream.clear();
    input_stream.seekg(0);
    {
      auto hash_function = multihash::function(multihash::code::sha2_256);
      auto hash = hash_function(std::istream_iterator<char>(input_stream),
                                std::istream_iterator<char>());
      {
        auto expected = static_cast<unsigned char>(multihash::code::sha2_256);
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
        hash = hash_function(input.begin(), input.end());
        result = toHexString(hash);
        EXPECT_EQ(expected, result);
      }
    }
    input_stream.clear();
    input_stream.seekg(0);
    {
      auto hash_function = multihash::function(multihash::code::sha2_512);
      auto hash = hash_function(std::istream_iterator<char>(input_stream),
                                std::istream_iterator<char>());
      {
        auto expected = static_cast<unsigned char>(multihash::code::sha2_512);
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
    input_stream.clear();
    input_stream.seekg(0);
    {
      auto hash_function = multihash::function(multihash::code::sha3_256);
      auto hash = hash_function(std::istream_iterator<char>(input_stream),
                                std::istream_iterator<char>());
      {
        auto expected = static_cast<unsigned char>(multihash::code::sha3_256);
        auto result = static_cast<unsigned char>(hash.code());
        EXPECT_EQ(expected, result);
      }
      {
        auto expected =
            "162076d3bc41c9f588f7fcd0d5bf4718f8f84b1c41b20882703100b9eb9413807c"
            "0"
            "1";
        auto result = toHexString(hash);
        EXPECT_EQ(expected, result);
      }
    }
  }
  {
    {
      auto source_chars =
          std::string("All work and no play makes Jack a dull boy.\n");
      auto repeat_count = 237;
      std::stringstream os;
      for (auto i = 0; i < repeat_count; ++i) {
        os << source_chars;
      }
      auto input = os.str();
      multihash::function hash_function(multihash::code::sha1);
      auto hash = hash_function(input.begin(), input.end());
      auto expected = "1114da96826ae365682f4123273c88e81479e899d8ac";
      auto result = toHexString(hash);
      EXPECT_EQ(expected, result);

      // hash again
      hash = hash_function(input.begin(), input.end());
      result = toHexString(hash);
      EXPECT_EQ(expected, result);
    }
  }
}

TEST(multihash, encoding) {
  std::istringstream input_stream("foo");
  multihash::function hash_function(multihash::code::sha1);
  auto hash = hash_function(std::istream_iterator<char>(input_stream),
                            std::istream_iterator<char>());
  auto view = std::string_view(hash.data(), hash.size());
  auto decoded = multihash::digest(view);
  EXPECT_EQ(hash, decoded);
}

TEST(multihash, default_values) {
  multihash::function hash_function;
  EXPECT_EQ(multihash::code::sha2_256, hash_function.code());
}

TEST(multihash, inequality) {
  multihash::function hash;
  std::string foo = "foo";
  auto foo_hash = hash(foo.begin(), foo.end());
  std::string bar = "bar";
  auto bar_hash = hash(bar.begin(), bar.end());
  EXPECT_NE(foo_hash, bar_hash);
  multihash::digest mh{std::string{"blah"}};
  EXPECT_NE(mh, foo_hash);
  EXPECT_EQ(mh, mh);
  EXPECT_EQ(foo_hash, foo_hash);
  bar_hash = foo_hash;
  EXPECT_EQ(foo_hash, bar_hash);
}

TEST(multihash, hash_construction) {
  multihash::function hash_function;
  std::string foo("foo");
  auto hash = hash_function(foo.begin(), foo.end());
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
    multihash::digest hash_copied(hash);
    EXPECT_EQ(expected, toHexString(hash));
    EXPECT_EQ(hash_copied, hash);
  }

  EXPECT_EQ(hash_function(foo.begin(), foo.end()),
            hash_function(foo.begin(), foo.end()));
  EXPECT_EQ(hash, hash_function(foo.begin(), foo.end()));
  EXPECT_EQ(expected, toHexString(hash_function(foo.begin(), foo.end())));

  auto varint = multihash::const_varint<0x02u>{};
  auto code = multihash::code::from_value(varint);
  auto expected_view = std::string_view(expected);
  auto buffer = std::string{};
  buffer.resize(expected.size() + 2);
  auto view = multihash::string_span(buffer);
  {
    multihash::digest<multihash::string_span> h(code, expected_view, view);
    EXPECT_EQ(code, h.code());
    EXPECT_EQ(expected.size(), h.value().size());
    EXPECT_EQ(expected, h.value());
  }
  {
    multihash::digest<std::string> h(code, expected_view);
    EXPECT_EQ(code, h.code());
    EXPECT_EQ(expected.size(), h.value().size());
    EXPECT_EQ(expected, h.value());
  }
}
