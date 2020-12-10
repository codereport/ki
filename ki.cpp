// https://www.godbolt.org/z/djz3YG

#include <vector>
#include <numeric>
#include <algorithm>

#include <fmt/core.h>
#include <fmt/format.h>

namespace ki {

// FILTER
template <typename Pred>
struct filter_proxy { Pred predicate; };

template <typename Pred>
auto filter(Pred predicate) { return filter_proxy{predicate}; }

template <typename Range, typename Pred>
auto operator|(Range r, filter_proxy<Pred> proxy) -> Range {
    auto it = std::remove_if(r.begin(), r.end(), proxy.predicate);
    r.erase(it, r.end());
    return r;
}

// MAP
template <typename UnaryOp>
struct map_proxy { UnaryOp unary_op; };

template <typename UnaryOp>
auto map(UnaryOp unary_op) { return map_proxy{unary_op}; }

template <typename Range, typename UnaryOp>
auto operator|(Range r, map_proxy<UnaryOp> proxy) -> Range {
    std::transform(r.begin(), r.end(), r.begin(), proxy.unary_op);
    return r;
}

// FOLD
template <typename T, typename BinaryOp>
struct fold_proxy { 
    T init;
    BinaryOp binary_op; 
};

template <typename T, typename BinaryOp>
auto fold(T t, BinaryOp binary_op) { return fold_proxy{t, binary_op}; }

template <typename Range, typename T, typename BinaryOp>
auto operator|(Range r, fold_proxy<T, BinaryOp> proxy) -> T {
    return std::accumulate(r.begin(), r.end(), proxy.init, proxy.binary_op);
}

// IOTA
struct iota_proxy {};
auto iota() { return iota_proxy{}; }

template <typename T>
auto operator|(T t, iota_proxy proxy) -> std::vector<int> {
    std::vector<int> v(t);
    std::iota(v.begin(), v.end(), 0);
    return v;
}

// REVERSE
struct reverse_proxy {};
auto reverse() { return reverse_proxy{}; }

template <typename Range>
auto operator|(Range r, reverse_proxy proxy) -> Range {
    std::reverse(r.begin(), r.end());
    return r;
}

// TAKE
struct take_proxy { int n; };
auto take(int n) { return take_proxy{n}; }

template <typename Range>
auto operator|(Range r, take_proxy proxy) -> Range {
    r.erase(r.begin() + proxy.n, r.end());
    return r;
}

// DROP
struct drop_proxy { int n; };
auto drop(int n) { return drop_proxy{n}; }

template <typename Range>
auto operator|(Range r, drop_proxy proxy) -> Range {
    r.erase(r.begin(), r.begin() + proxy.n);
    return r;
}

}

auto main() -> int {

    auto v = std::vector{1, 2, 3, 4, 5};

    auto const result = 
        v | ki::filter([](auto e) { return e % 2 == 0; }) 
          | ki::map([](auto e) { return e + 1; }) 
          | ki::fold(0, std::plus{})
          | ki::iota()
          | ki::reverse()
          | ki::drop(4)
          | ki::take(2)
          | ki::fold(1, std::multiplies{});

    // fmt::print("[{}]\n", fmt::join(result, ","));
    fmt::print("{}\n", result);

    return 0;
}
