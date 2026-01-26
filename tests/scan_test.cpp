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
 * @brief Проверка конвертации в const тип
 * @details Ошибки не ожидается
 */
TEST(ScanFormatTest, CheckSupportConst) {

    std::string input = "123";
    std::string fmt = "{%d}";

    auto result_int = stdx::scan<const int>(input, fmt);
    ASSERT_TRUE(result_int);
    ASSERT_EQ(std::tuple_size_v<decltype(result_int.value().values)>, 1);
    EXPECT_EQ(result_int.value().value<0>(), 123);

    fmt = "{%s}";
    auto result_str = stdx::scan<const std::string>(input, fmt);
    ASSERT_TRUE(result_str);
    ASSERT_EQ(std::tuple_size_v<decltype(result_str.value().values)>, 1);
    EXPECT_EQ(result_str.value().value<0>(), "123");
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
 * @brief Проверка преобразования строки с с несколькими значениями
 * @details Ошибки не ожидается
 */
TEST(ScanMultiArgsTest, CheckMultiFormat) {
    std::string input = "123 text -12 and +8 numbers 3.14";
    std::string fmt = "{%u} {%s} {%d} {%s} {%s} {%s} {%f}";
    auto result = stdx::scan<uint8_t, std::string, int16_t, std::string, std::string, std::string, float>(input, fmt);
    ASSERT_TRUE(result);
    ASSERT_EQ(std::tuple_size_v<decltype(result.value().values)>, 7);
    EXPECT_EQ(result.value().value<0>(), 123);
    EXPECT_EQ(result.value().value<1>(), "text");
    EXPECT_EQ(result.value().value<2>(), -12);
    EXPECT_EQ(result.value().value<3>(), "and");
    EXPECT_EQ(result.value().value<4>(), "+8");
    EXPECT_EQ(result.value().value<5>(), "numbers");
    EXPECT_FLOAT_EQ(result.value().value<6>(), 3.14);
}

/**
 * @brief Проверка преобразования строки с с несколькими значениями
 * @details Ошибки не ожидается
 */
TEST(ScanMultiArgsTest, CheckMultiEmptyFormat) {
    std::string input = "123 text -12 and +8 numbers 3.14";
    std::string fmt = "{} {} {} {} {} {} {}";
    auto result = stdx::scan<uint8_t, std::string, int16_t, std::string, std::string, std::string, float>(input, fmt);
    ASSERT_TRUE(result);
    ASSERT_EQ(std::tuple_size_v<decltype(result.value().values)>, 7);
    EXPECT_EQ(result.value().value<0>(), 123);
    EXPECT_EQ(result.value().value<1>(), "text");
    EXPECT_EQ(result.value().value<2>(), -12);
    EXPECT_EQ(result.value().value<3>(), "and");
    EXPECT_EQ(result.value().value<4>(), "+8");
    EXPECT_EQ(result.value().value<5>(), "numbers");
    EXPECT_FLOAT_EQ(result.value().value<6>(), 3.14);
}

/**
 * @brief Проверка преобразования строки с с несколькими значениями
 * @details Ошибки не ожидается
 */
TEST(ScanMultiArgsTest, CheckMultiMixFormat) {
    std::string input = "123 text -12 and +8 numbers 3.14";
    std::string fmt = "{%d} {%s} {} {} {%s} {} {}";
    auto result =
        stdx::scan<const uint8_t, const std::string, int16_t, std::string, std::string, std::string, float>(input, fmt);
    ASSERT_TRUE(result);
    ASSERT_EQ(std::tuple_size_v<decltype(result.value().values)>, 7);
    EXPECT_EQ(result.value().value<0>(), 123);
    EXPECT_EQ(result.value().value<1>(), "text");
    EXPECT_EQ(result.value().value<2>(), -12);
    EXPECT_EQ(result.value().value<3>(), "and");
    EXPECT_EQ(result.value().value<4>(), "+8");
    EXPECT_EQ(result.value().value<5>(), "numbers");
    EXPECT_FLOAT_EQ(result.value().value<6>(), 3.14);
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
TEST(ScanFailTest, CheckRefType) {

    std::string input = "123abc";
    std::string fmt = "{%d}";

    auto result = stdx::scan<int &>(input, fmt);
    ASSERT_FALSE(result);
}

/**
 * @brief Проверка конвертации в const тип
 * @details Ожидается ошибка
 */
TEST(ScanFailTest, CheckConstRefType) {

    std::string input = "123abc";
    std::string fmt = "{%d}";

    auto result = stdx::scan<const int &>(input, fmt);
    ASSERT_FALSE(result);
}

/**
 * @brief Проверка преобразования строки с с несколькими значениями и неправильно указанным спецификатором
 * @details Ожидается ошибка
 */
TEST(ScanFailTest, CheckInvalidMultiFormat) {
    std::string input = "123 text -12 and +8 numbers 3.14";
    //                   v    v    x
    std::string fmt = "{%d} {%s} {%u} {} {} {} {}";
    auto result =
        stdx::scan<const uint8_t, const std::string, int16_t, std::string, std::string, std::string, float>(input, fmt);
    EXPECT_FALSE(result);
}