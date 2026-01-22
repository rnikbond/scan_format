#include <gtest/gtest.h>
#include <print>

#include "scan.hpp"

/**
 * @brief Проверка преобразования строки в int
 * @details Ошибки не ожидается
 */
TEST(ScanFormatTest, CheckSupport_int) {
    std::string input = "123";
    std::string fmt = "{%d}";
    auto result = stdx::scan<int>(input, fmt);
    ASSERT_TRUE(result);
    ASSERT_EQ(std::tuple_size_v<decltype(result.value().values)>, 1);
    EXPECT_EQ(result.value().value<0>(), 123);
}

/**
 * @brief Проверка преобразования строки в double
 * @details Ошибки не ожидается
 */
TEST(ScanFormatTest, CheckSupport_double) {
    std::string input = "123.321";
    std::string fmt = "{%f}";
    auto result = stdx::scan<double>(input, fmt);
    ASSERT_TRUE(result);
    ASSERT_EQ(std::tuple_size_v<decltype(result.value().values)>, 1);
    EXPECT_DOUBLE_EQ(result.value().value<0>(), 123.321);
}

/**
 * @brief Проверка преобразования строки в string
 * @details Ошибки не ожидается
 */
TEST(ScanFormatTest, CheckSupport_string) {
    std::string input = "text text text";
    std::string fmt = "{%s}";
    auto result = stdx::scan<std::string>(input, fmt);
    ASSERT_TRUE(result);
    ASSERT_EQ(std::tuple_size_v<decltype(result.value().values)>, 1);
    EXPECT_EQ(result.value().value<0>(), input);
}

/**
 * @brief Проверка выхода преобразованного значения за диапазон типа
 * @details Ожидается ошибка
 */
TEST(ScanFormatTest, Check_int8_OutOfRange) {
    std::string input = "200";
    std::string fmt = "{%d}";
    auto result = stdx::scan<int8_t>(input, fmt);
    EXPECT_FALSE(result);
}

/**
 * @brief  Проверка преобразования строки в int без указания формата
 * @details Ошибки не ожидается
 */
TEST(ScanEmptyFormatTest, CheckSupport_int) {
    std::string input = "123";
    std::string fmt = "{}";
    auto result = stdx::scan<int>(input, fmt);
    ASSERT_TRUE(result);
    ASSERT_EQ(std::tuple_size_v<decltype(result.value().values)>, 1);
    EXPECT_EQ(result.value().value<0>(), 123);
}

/**
 * @brief  Проверка преобразования строки в int без указания формата
 * @details Ошибки не ожидается
 */
TEST(ScanEmptyFormatTest, CheckSupport_double) {
    std::string input = "123.321";
    std::string fmt = "{}";
    auto result = stdx::scan<double>(input, fmt);
    ASSERT_TRUE(result);
    ASSERT_EQ(std::tuple_size_v<decltype(result.value().values)>, 1);
    EXPECT_DOUBLE_EQ(result.value().value<0>(), 123.321);
}

/**
 * @brief Проверка преобразования строки в string без указания формата
 * @details Ошибки не ожидается
 */
TEST(ScanEmptyFormatTest, CheckSupport_string) {
    std::string input = "text text text";
    std::string fmt = "{}";
    auto result = stdx::scan<std::string>(input, fmt);
    ASSERT_TRUE(result);
    ASSERT_EQ(std::tuple_size_v<decltype(result.value().values)>, 1);
    EXPECT_EQ(result.value().value<0>(), input);
}

/**
 * @brief Проверка невозможности преобразования строки в указанный формат
 * @details Ожидается ошибка
 */
TEST(ScanFailTest, CheckImpossibleParseInputToType) {
    std::string input = "text text text";

    auto result1 = stdx::scan<int>(input, "%d");
    ASSERT_FALSE(result1);

    auto result2 = stdx::scan<float>(input, "%f");
    ASSERT_FALSE(result2);

    auto result3 = stdx::scan<double>(input, "%d");
    ASSERT_FALSE(result2);
}

/**
 * @brief Проверка несоответствия типа и формата
 * @details Ожидается ошибка
 */
TEST(ScanFailTest, CheckDiffTypeAndFormat) {
    std::string input = "321";

    auto result_int = stdx::scan<int>(input, "%s");
    ASSERT_FALSE(result_int);

    auto result_float = stdx::scan<float>(input, "%d");
    ASSERT_FALSE(result_float);

    auto result_str = stdx::scan<std::string>(input, "%u");
    ASSERT_FALSE(result_str);
}

/**
 * @brief Проверка несуществующего формата
 * @details Ожидается ошибка
 */
TEST(ScanFailTest, CheckUnknownFormat) {
    std::string input = "321";

    std::vector<std::string> formats = {"%k", "%y", "%D", "%f", "$f", "#d", "%d", "%f", "%u"};
    for (auto &fmt : formats) {
        auto result = stdx::scan<int>(input, fmt);
        ASSERT_FALSE(result);
    }
}

/**
 * @brief Проверка конвертации строки в число, которая начинается с цифр и заканчивается буквами
 * @details Ожидается ошибка
 *
 * Тест показывает наличие проверки указателя, который возвращает std::from_chars
 */
TEST(ScanFailTest, CheckPartStrToInt) {

    std::string input = "123abc";
    std::string fmt = "{%d}";

    auto result = stdx::scan<int>(input, fmt);
    ASSERT_FALSE(result);
}

/**
 * @brief Проверка конвертации в ссылочный тип
 * @details Ожидается ошибка
 */
TEST(ScanFailTest, CheckConstType) {

    std::string input = "123abc";
    std::string fmt = "{%d}";

    auto result = stdx::scan<int &>(input, fmt);
    ASSERT_FALSE(result);
}
