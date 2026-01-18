#pragma once

#include <charconv>
#include <concepts>
#include <cstdlib>
#include <expected>
#include <format>
#include <string>
#include <string_view>
#include <system_error>
#include <type_traits>
#include <utility>
#include <vector>

#include "types.hpp"

namespace stdx::details {

/**
 * @brief Концепт проверяет, является ли тип целочисленным, вещественным или строкой
 * @tparam T Проверяемый тип данных
 */
template <typename T>
concept parsable = std::is_integral_v<T> || std::is_floating_point_v<T> || std::same_as<T, std::string_view> ||
                   std::same_as<T, std::string>;

/**
 * @brief Концепт для проверки натуральных чисел
 * @tparam T Проверяемый тип данных
 */
template <typename T>
concept is_natural = std::is_integral_v<T> && !std::same_as<T, bool>;

template <parsable T>
constexpr std::expected<T, scan_error> parse_numerical(std::string_view input) {

    T value;
    auto [_, err] = std::from_chars(input.data(), input.data() + input.size(), value);
    if (err != std::errc{}) {
        std::error_code code = std::make_error_code(err);
        return std::unexpected(scan_error{std::format("failed parse to {}: {}", typeid(T).name(), code.message())});
    }

    return value;
}

/**
 * @brief Преобразование строки в тип, указанный в формате
 *
 * @tparam T    Один из преобразуемых типов
 * @param input Входные данные, которые нужно преобразовать
 * @param fmt   Формат
 * @return      Значение, преобразованное в тип, или ошибку
 *
 * Список преобразуемых типов указан в концепте @a parsable
 */
template <parsable T>
constexpr std::expected<T, scan_error> parse_value_to_fmt(std::string_view input, std::string_view fmt) {

    if (fmt[0] != '%' || fmt.length() != 2) {
        return std::unexpected(scan_error{std::format("invalid fmt: {}", fmt)});
    }

    switch (fmt[1]) {
    case 's': {
        if constexpr (std::same_as<T, std::string> || std::same_as<T, std::string_view>) {
            return std::string{input};
        }
        return std::unexpected(scan_error{std::format("invalid specifier for type: {}", typeid(T).name())});
    }
    case 'd': {
        if constexpr (std::is_integral_v<T>) {
            return parse_numerical<T>(input);
        }

        return std::unexpected(scan_error{std::format("invalid specifier for type: {}", typeid(T).name())});
    }
    case 'u': {
        if constexpr (is_natural<T>) {
            return parse_numerical<T>(input);
        }
        return std::unexpected(scan_error{std::format("invalid specifier for type: {}", typeid(T).name())});
    }
    case 'f': {
        if constexpr (std::is_floating_point_v<T>) {
            return parse_numerical<T>(input);
        }
        return std::unexpected(scan_error{std::format("invalid specifier for type: {}", typeid(T).name())});
    }
    default: {
        return std::unexpected(scan_error{std::format("unknown format: {}", fmt)});
    }
    }
}

/**
 * @brief Преобразование строки в тип, указанный в шаблонном вызове
 *
 * @tparam T    Тип
 * @param input Входные данные, которые нужно преобразовать
 * @return      Значение, преобразованное в тип, или ошибку
 */
template <parsable T>
constexpr std::expected<T, scan_error> parse_value_auto_fmt(std::string_view input) {

    if constexpr (is_natural<T> || std::is_floating_point_v<T>) {
        return parse_numerical<T>(input);
    } else if constexpr (std::same_as<T, std::string> || std::same_as<T, std::string_view>) {
        return std::string{input};
    }

    return std::unexpected(scan_error{"not implemented"});
}

/**
 * @brief Функция для парсинга значения с учетом спецификатора формата
 *
 * @tparam T    Один из типов, который можно преобразовать
 * @param input Входные данные, которые нужно преобразовать
 * @param fmt   Формат, в который нужно преобразовать
 * @return      Значение, преобразованное в тип указанного формата, или ошибку
 */
template <parsable T>
std::expected<T, scan_error> constexpr parse_value_with_format(std::string_view input, std::string_view fmt) {

    if (fmt.length() == 0) {
        return parse_value_auto_fmt<T>(input);
    } else {
        return parse_value_to_fmt<T>(input, fmt);
    }
}

// Функция для проверки корректности входных данных и выделения из обеих строк интересующих данных для парсинга
template <typename... Ts>
std::expected<std::pair<std::vector<std::string_view>, std::vector<std::string_view>>, scan_error>
parse_sources(std::string_view input, std::string_view format) {
    std::vector<std::string_view> format_parts;  // Части формата между {}
    std::vector<std::string_view> input_parts;
    size_t start = 0;
    while (true) {
        size_t open = format.find('{', start);
        if (open == std::string_view::npos) {
            break;
        }
        size_t close = format.find('}', open);
        if (close == std::string_view::npos) {
            break;
        }

        // Если между предыдущей } и текущей { есть текст,
        // проверяем его наличие во входной строке
        if (open > start) {
            std::string_view between = format.substr(start, open - start);
            auto pos = input.find(between);
            if (input.size() < between.size() || pos == std::string_view::npos) {
                return std::unexpected(scan_error{"Unformatted text in input and format string are different"});
            }
            if (start != 0) {
                input_parts.emplace_back(input.substr(0, pos));
            }

            input = input.substr(pos + between.size());
        }

        // Сохраняем спецификатор формата (то, что между {})
        format_parts.push_back(format.substr(open + 1, close - open - 1));
        start = close + 1;
    }

    // Проверяем оставшийся текст после последней }
    if (start < format.size()) {
        std::string_view remaining_format = format.substr(start);
        auto pos = input.find(remaining_format);
        if (input.size() < remaining_format.size() || pos == std::string_view::npos) {
            return std::unexpected(scan_error{"Unformatted text in input and format string are different"});
        }
        input_parts.emplace_back(input.substr(0, pos));
        input = input.substr(pos + remaining_format.size());
    } else {
        input_parts.emplace_back(input);
    }
    return std::pair{format_parts, input_parts};
}

}  // namespace stdx::details