#include "../include/memoize.h"
#include <iostream>


int f(int x) {
    return x * 100;
}

struct Foo {

    int value;

    int operator()(int v) const {
        return value * v; 
    }

    int operator()(int v, double x) const {
        return static_cast<int>(this->operator()(v) * x);
    }

};

int main() {
    auto mem = memo::memoize(f);
    const auto& result = mem(10);
    const auto& result2 = mem(10);
    std::cout << "result = " << result << std::endl;
    std::cout << "result2 = " << result2 << std::endl;

    //auto xx = std::move(mem);

    auto m = memo::memoize<int(int)>(Foo{5});
    const auto& r = m(10);

    std::cout << "m = " << r << std::endl;

    auto mm = memo::memoize<int(int, double)>(Foo{5});
    std::cout << "mm = " << mm(10, 0.78) << std::endl;

    auto f = [&m, &mm](int value, double mul) { return mm(m(value), mul); };
    auto mem3 = memo::memoize(f);
    std::cout << mem3(10, 0.98) << std::endl;

    return 0;
}