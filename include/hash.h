#pragma once

#include <tuple>
#include <functional>


namespace memo::detail {

template <typename Tuple>
struct Hash final {
    std::size_t operator()(const Tuple& values) const noexcept {
        std::size_t seed = 0;
        computeHash<std::tuple_size_v<Tuple> - 1>(seed, values);

        return seed;
    }

    template <std::size_t index>
    constexpr static void computeHash(std::size_t& seed, const Tuple& values) noexcept {
        if constexpr (index == 0) {
            computeHash(seed, std::get<index>(values));
        }
        else {
            computeHash<index - 1>(seed, values);
            computeHash(seed, std::get<index>(values));
        }
    }

    template <typename T>
    constexpr static void computeHash(std::size_t& seed, const T& value) noexcept {
        seed ^= std::hash<T>{}(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
};

}