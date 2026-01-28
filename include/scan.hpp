#pragma once

#include <expected>
#include <print>
#include <ranges>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include "parse.hpp"
#include "types.hpp"

namespace stdx {

/**
 * @brief Концепт содержит проверки типов, которые поддерживаются для сканирования
 * @tparam T Тип, который нужно проверить
 */
template <typename T>
concept SupportTypes =
    !std::is_reference_v<T> && !std::same_as<T, bool> &&
    (std::is_integral_v<T> || std::is_floating_point_v<T> || std::same_as<std::remove_cv_t<T>, std::string_view> ||
     std::same_as<std::remove_cv_t<T>, std::string>);

/**
 * @brief Выполняет парсинг последовательности строк в указанные форматы
 *
 * @tparam Ts     Список типов, в которые нужно преобразовать
 * @tparam I      Последовательность индексов
 * @param values  Входной список строк, которые нужно преобразовать
 * @param formats Форматы, в которые нужно преобразовать
 * @return std::expected, которые содержит данные или ошибку с описанием
 */
template <SupportTypes... Ts, std::size_t... I>
std::expected<std::tuple<Ts...>, details::scan_error> scan_sequence(const std::vector<std::string_view> &values,
                                                                    const std::vector<std::string_view> &formats,
                                                                    std::index_sequence<I...>) {

    std::tuple<std::decay_t<Ts>...> result;

    bool has_error = false;
    details::scan_error err;

    (
        [&] {
            if (has_error) {
                return;
            }

            using T = std::tuple_element_t<I, std::tuple<std::decay_t<Ts>...>>;
            auto result_at = details::parse_value_with_format<T>(values[I], formats[I]);
            if (result_at) {
                std::get<I>(result) = *result_at;
            } else {
                err = result_at.error();
                has_error = true;
            }
        }(),
        ...);

    if (has_error) {
        return std::unexpected(err);
    }
    return result;
}

/**
 * @brief Выполняет преобразование значений из входной строки в указанные форматы

 * @tparam Ts    Список типов, в которые нужно преобразовать
 * @param input  Входные данные
 * @param format Форматы в виде строки, в которые нужно преобразовать
 * @return std::expected, который содержит std::tuple со значениями или ошибку, если что-то пошло не так
 *
 * Обработка типов, которые не поддерживаются концептом @a SupportTypes, вынесена в перегрузку шаблона функции
 */
template <SupportTypes... Ts>
std::expected<details::scan_result<Ts...>, details::scan_error> scan(std::string_view input, std::string_view format) {

    auto parsed = details::parse_sources(input, format);
    if (!parsed) {
        return std::unexpected(parsed.error());
    }

    auto [formats, values] = std::move(*parsed);
    if (values.size() != formats.size() || values.size() != sizeof...(Ts)) {
        return std::unexpected(details::scan_error{"mismatch count formats and types"});
    }

    auto result = scan_sequence<Ts...>(values, formats, std::index_sequence_for<Ts...>{});
    if (!result) {
        return std::unexpected(result.error());
    }

    return details::scan_result<Ts...>{std::move(*result)};
}

/**
 * @brief Перегрузка функции преобразования для всех неподдерживаемых типов
 *
 * @tparam Ts    Список типов, в которые нужно преобразовать
 * @param input  Входные данные
 * @param format Форматы в виде строки, в которые нужно преобразовать
 * @return Всегда ошибка о неподдерживаемом типе
 */
template <typename... Ts>
std::expected<details::scan_result<Ts...>, details::scan_error> scan(std::string_view input, std::string_view format) {
    return std::unexpected(details::scan_error{"type is not supported"});
}

}  // namespace stdx
