#include "hash.h"
#include <unordered_map>


namespace memo {

template <typename Signature, typename Function>
class Memoizer;

template <typename Return, typename ...Args, typename Funcion>
class Memoizer<Return(Args...), Funcion> final {
public:
    using ReturnType = Return;
    using ConstRef = const ReturnType&;
    using FunctionType = Funcion;
    using ArgsType = std::tuple<std::remove_reference_t<Args>...>;
    using CacheType = std::unordered_map<ArgsType, ReturnType, detail::Hash<ArgsType>>;

public:
    Memoizer(const Memoizer&) = delete;
    Memoizer& operator=(const Memoizer&) = delete;

    constexpr Memoizer(const FunctionType& _func)
        : func{ _func }
        {}

    Memoizer(Memoizer&& other)
        : func{ std::move(other.func) }
        , cache{ std::move(other.cache) }  {
    }

    ConstRef operator()(Args ...args) {
        const auto key = std::make_tuple(args...);
        if (const auto t = cache.find(key); t != cache.cend()) {
            return t->second;
        }

        decltype(auto) result = std::apply(func, key);
        decltype(auto) item = cache.emplace(key, result);

        return item.first->second;
    }

private:
    FunctionType func;
    CacheType cache;
};

}