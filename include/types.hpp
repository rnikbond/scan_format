#pragma once

#include <tuple>

namespace stdx::details {

// Класс для хранения ошибки неуспешного сканирования

struct scan_error {
    std::string message;
};

// Шаблонный класс для хранения результатов успешного сканирования

template <typename... Ts>
struct scan_result {
    std::tuple<Ts...> values;

    template <std::size_t I>
    const auto &value() const {
        return std::get<I>(values);
    }
};

}  // namespace stdx::details
