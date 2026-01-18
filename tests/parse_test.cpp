#include <gtest/gtest.h>

#include "parse.hpp"

TEST(ParseFormatTest, CheckSupport_int) {
    std::string input = "123";
    std::string fmt = "%d";
    auto result = stdx::details::parse_value_with_format<int>(input, fmt);
    ASSERT_TRUE(result);
    EXPECT_EQ(result.value(), 123);
}

TEST(ParseFormatTest, CheckSupport_double) {
    std::string input = "123.321";
    std::string fmt = "%f";
    auto result = stdx::details::parse_value_with_format<double>(input, fmt);
    ASSERT_TRUE(result);
    EXPECT_EQ(result.value(), 123.321);
}

TEST(ParseFormatTest, CheckSupport_string) {
    std::string input = "text text text";
    std::string fmt = "%s";
    auto result = stdx::details::parse_value_with_format<std::string>(input, fmt);
    ASSERT_TRUE(result);
    EXPECT_EQ(result.value(), input);
}

TEST(ParseFormatTest, Check_int8_OutOfRange) {
    std::string input = "200";  // INT_MAX + 1
    std::string fmt = "%d";
    auto result = stdx::details::parse_value_with_format<int8_t>(input, fmt);
    EXPECT_FALSE(result);
}
