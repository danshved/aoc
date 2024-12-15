#ifndef __AOC_COLLECTIONS_H__
#define __AOC_COLLECTIONS_H__

#include <cassert>
#include <iterator>
#include <tuple>
#include <utility>
#include <vector>

// Transposes a matrix. Could be a std::vector<std::vector<T>>
// or std::vector<std::string>.
template <typename C>
std::vector<C> Transpose(const std::vector<C>& v) {
    int height = v.size();
    assert(height > 0);
    int width = v[0].size();
    assert(width > 0);
    for (int i = 0; i < height; i++) {
        assert(v[i].size() == width);
    }

    std::vector<C> result;
    result.reserve(width);
    for (int j = 0; j < width; j++) {
        C result_row;
        result_row.reserve(height);
        for (int i = 0; i < height; i++) {
            result_row.push_back(v[i][j]);
        }
        result.push_back(std::move(result_row));
    }
    return result;
}

// Concatenates a container of containers into a single container of the same
// type as the inner containers.
template <typename C>
C::value_type Concat(const C& xss) {
    typename C::value_type result;
    for (const auto& xs : xss) {
        std::copy(std::begin(xs), std::end(xs), std::back_inserter(result));
    }
    return result;
}

// Applies a container-returning function to each element of the container and
// concatenates the results.
//
// The result is the same type of container that the function returns. If it
// returns vectors, the result is a vector. If it returns strings, a string.
template <typename C, typename F>
auto ConcatMap(const C& xs, F f) -> decltype(f(*std::begin(xs))) {
    using OutputC = decltype(f(*std::begin(xs)));
    OutputC result;
    for (const auto& x : xs) {
        OutputC ys = f(x);
        std::copy(std::begin(ys), std::end(ys), std::back_inserter(result));
    }
    return result;
}

// Finds a specific element in a multi-dimensional container. The first
// template parameter (n) is mandatory.
//
// The result is an std::optional of an n-tuple of ints. It is nullopt if
// the element isn't found, and an actual tuple of the first occurrence if
// it is found.
template <size_t n, typename Cont, typename T>
auto Find(const Cont& cont, const T& t) {
    return FindIf<n, Cont>(cont, [&t](const T& u) { return u == t; });
}

// Like Find(), but matches a predicate.
template <size_t n, typename Cont, typename Pred>
auto FindIf(const Cont& cont, Pred&& pred) {
    if constexpr (n == 0) {
        using ResType = std::optional<std::tuple<>>;
        return pred(cont) ? ResType(std::make_tuple()) : ResType(std::nullopt);
    } else {
        using ResType = decltype(std::optional(
            std::tuple_cat(std::make_tuple((int)0),
                           *FindIf<n - 1>(cont[0], std::forward<Pred>(pred)))));
        for (int i = 0; i < cont.size(); i++) {
            auto res = FindIf<n - 1>(cont[i], std::forward<Pred>(pred));
            if (res.has_value()) {
                return ResType(std::tuple_cat(std::make_tuple(i), *std::move(res)));
            }
        }
        return ResType(std::nullopt);
    }
}

template <size_t n, typename Cont, typename T>
auto FindOrDie(const Cont& cont, const T& t) {
    auto res = Find<n>(cont, t);
    assert(res.has_value());
    return *std::move(res);
}

template <size_t n, typename Cont, typename Pred>
auto FindIfOrDie(const Cont& cont, Pred&& pred) {
    auto res = FindIf<n>(cont, std::forward<Pred>(pred));
    assert(res.has_value());
    return *std::move(res);
}

#endif