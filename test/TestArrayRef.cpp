#include "gtest/gtest.h"
#include <multihash/ArrayRef.h>

TEST(ArrayRef, ArrayRef)
{
    auto expected = std::string("hello, world!");
    auto ar = multihash::ArrayRef(expected);
    auto result = static_cast<std::string>(ar);
    decltype(ar.size()) length = 13;
    EXPECT_EQ(expected, result);
    EXPECT_EQ(length, ar.size());
    EXPECT_EQ(length, result.size());
}
