#include <gtest/gtest.h>

#include "parse.hpp"

/**
 * @brief Проверка преобразования строки в int
 * @details Ошибки не ожидается
 */
TEST(ParseFormatTest, CheckSupport_int) {
    std::string input = "123";
    std::string fmt = "%d";
    auto result = stdx::details::parse_value_with_format<int>(input, fmt);
    ASSERT_TRUE(result);
    EXPECT_EQ(result.value(), 123);
}

/**
 * @brief Проверка преобразования строки в double
 * @details Ошибки не ожидается
 */
TEST(ParseFormatTest, CheckSupport_double) {
    std::string input = "123.321";
    std::string fmt = "%f";
    auto result = stdx::details::parse_value_with_format<double>(input, fmt);
    ASSERT_TRUE(result);
    EXPECT_EQ(result.value(), 123.321);
}

/**
 * @brief Проверка преобразования строки в string
 * @details Ошибки не ожидается
 */
TEST(ParseFormatTest, CheckSupport_string) {
    std::string input = "text text text";
    std::string fmt = "%s";
    auto result = stdx::details::parse_value_with_format<std::string>(input, fmt);
    ASSERT_TRUE(result);
    EXPECT_EQ(result.value(), input);
}

/**
 * @brief Проверка выхода преобразованного значения за диапазон типа
 * @details Ожидается ошибка
 */
TEST(ParseFormatTest, Check_int8_OutOfRange) {
    std::string input = "200";
    std::string fmt = "%d";
    auto result = stdx::details::parse_value_with_format<int8_t>(input, fmt);
    EXPECT_FALSE(result);
}

/**
 * @brief  Проверка преобразования строки в int без указания формата
 * @details Ошибки не ожидается
 */
TEST(ParseEmptyFormatTest, CheckSupport_int) {
    std::string input = "123";
    std::string fmt = "";
    auto result = stdx::details::parse_value_with_format<int>(input, fmt);
    ASSERT_TRUE(result);
    EXPECT_EQ(result.value(), 123);
}

/**
 * @brief  Проверка преобразования строки в int без указания формата
 * @details Ошибки не ожидается
 */
TEST(ParseEmptyFormatTest, CheckSupport_double) {
    std::string input = "123.321";
    std::string fmt = "";
    auto result = stdx::details::parse_value_with_format<double>(input, fmt);
    ASSERT_TRUE(result);
    EXPECT_EQ(result.value(), 123.321);
}

/**
 * @brief Проверка преобразования строки в string без указания формата
 * @details Ошибки не ожидается
 */
TEST(ParseEmptyFormatTest, CheckSupport_string) {
    std::string input = "text text text";
    std::string fmt = "";
    auto result = stdx::details::parse_value_with_format<std::string>(input, fmt);
    ASSERT_TRUE(result);
    EXPECT_EQ(result.value(), input);
}

/**
 * @brief Проверка невозможности преобразования строки в указанный формат
 * @details Ожидается ошибка
 */
TEST(ParseFailTest, CheckImpossibleParseInputToType) {
    std::string input = "text text text";

    auto result = stdx::details::parse_value_with_format<int>(input, "%d");
    ASSERT_FALSE(result);

    result = stdx::details::parse_value_with_format<float>(input, "%f");
    ASSERT_FALSE(result);

    result = stdx::details::parse_value_with_format<double>(input, "%d");
    ASSERT_FALSE(result);
}

/**
 * @brief Проверка несоответствия типа и формата
 * @details Ожидается ошибка
 */
TEST(ParseFailTest, CheckDiffTypeAndFormat) {
    std::string input = "321";

    auto result = stdx::details::parse_value_with_format<int>(input, "%s");
    ASSERT_FALSE(result);

    result = stdx::details::parse_value_with_format<float>(input, "%d");
    ASSERT_FALSE(result);

    auto result_str = stdx::details::parse_value_with_format<std::string>(input, "%u");
    ASSERT_FALSE(result_str);
}

/**
 * @brief Проверка несуществующего формата
 * @details Ожидается ошибка
 */
TEST(ParseFailTest, CheckUnknownFormat) {
    std::string input = "321";

    auto result = stdx::details::parse_value_with_format<int>(input, "%k");
    ASSERT_FALSE(result);

    result = stdx::details::parse_value_with_format<int>(input, "%y");
    ASSERT_FALSE(result);

    result = stdx::details::parse_value_with_format<int>(input, "%D");
    ASSERT_FALSE(result);

    result = stdx::details::parse_value_with_format<int>(input, "$f");
    ASSERT_FALSE(result);
}
