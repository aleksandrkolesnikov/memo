#pragma once

#include "memoizer.h"


namespace memo {

template <typename Return, typename ...Args>
struct Signature;

template <typename Return, typename ...Args>
struct Signature<Return(&)(Args...)> {
    using Type = Return(Args...);
};

template <typename Return, typename ...Args>
struct Signature<Return(*)(Args...)> {
    using Type = Return(Args...);
};

template <typename T>
struct Signature<T> : Signature<decltype(&T::operator())> {
};

template <typename T, typename Return, typename ...Args>
struct Signature<Return(T::*)(Args...)> {
    using Type = Return(Args...);
};

template <typename T, typename Return, typename ...Args>
struct Signature<Return(T::*)(Args...) const> {
    using Type = Return(Args...);
};

template <typename T>
using SignatureType = typename Signature<T>::Type;

template <typename Function>
auto memoize(Function func) {
    return Memoizer<SignatureType<Function>, Function>{ std::move(func) };
}

template <typename Signature, typename Function>
auto memoize(Function func) {
    return Memoizer<Signature, Function>{ std::move(func) } ;
}

}
