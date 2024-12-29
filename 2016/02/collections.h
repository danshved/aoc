#ifndef __AOC_COLLECTIONS_H__
#define __AOC_COLLECTIONS_H__

#include <cassert>
#include <iterator>
#include <optional>
#include <ranges>
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

template <size_t n, typename T>
struct NTupleHelper {
    using Type = decltype(std::tuple_cat(
        std::make_tuple(std::declval<T>()),
        std::declval<typename NTupleHelper<n - 1, T>::Type>()));
};

template <typename T>
struct NTupleHelper<0, T> {
    using Type = std::tuple<>;
};

// Synonym for std::tuple<T, ... (n times) ..., T>.
template <size_t n, typename T>
using NTuple = NTupleHelper<n, T>::Type;

// Finds a specific element in a multi-dimensional container. The first
// template parameter (n) is mandatory.
//
// The result is an std::optional of an n-tuple of ints. It is nullopt if
// the element isn't found, and an actual tuple of the first occurrence if
// it is found.
template <size_t n, typename Cont, typename T>
std::optional<NTuple<n, int>> Find(const Cont& cont, const T& t) {
    return FindIf<n, Cont>(cont, [&t](const T& u) { return u == t; });
}

// Like Find(), but matches a predicate.
template <size_t n, typename Cont, typename Pred>
std::optional<NTuple<n, int>> FindIf(const Cont& cont, Pred&& pred) {
    if constexpr (n == 0) {
        if (pred(cont)) {
            return std::tuple<>();
        }
        return std::nullopt;
    } else {
        for (int i = 0; i < std::ranges::size(cont); i++) {
            std::optional<NTuple<n - 1, int>> res =
                FindIf<n - 1>(cont[i], std::forward<Pred>(pred));
            if (res.has_value()) {
                return std::tuple_cat(std::make_tuple(i), *std::move(res));
            }
        }
        return std::nullopt;
    }
}

// Convenience wrapper for Find(). Returns an n-tuple of ints, fails if the
// element isn't found.
template <size_t n, typename Cont, typename T>
NTuple<n, int> FindOrDie(const Cont& cont, const T& t) {
    std::optional<NTuple<n, int>> res = Find<n>(cont, t);
    assert(res.has_value());
    return *std::move(res);
}

// Convenience wrapper for FindIf(). Returns an n-tuple of ints, fails if the
// element isn't found.
template <size_t n, typename Cont, typename Pred>
NTuple<n, int> FindIfOrDie(const Cont& cont, Pred&& pred) {
    std::optional<NTuple<n, int>> res = FindIf<n>(cont, std::forward<Pred>(pred));
    assert(res.has_value());
    return *std::move(res);
}

template <size_t n, typename T>
struct NestedVectorHelper {
    using Type = std::vector<typename NestedVectorHelper<n - 1, T>::Type>;
};

template <typename T>
struct NestedVectorHelper<0, T> {
    using Type = T;
};

// Synonym for std::vector<std::vector<... (n times) ...<std::vector<T>>...>>.
template <size_t n, typename T>
using NestedVector = NestedVectorHelper<n, T>::Type;

// Creates a multidimensional vector of given shape filled with the given value.
template <typename T>
NestedVector<0, T> ConstVector(const T& val) {
    return val;
}

template <typename T, typename... Ts>
NestedVector<1 + sizeof...(Ts), T> ConstVector(const T& val, size_t size, Ts... sizes) {
    return std::vector(size, ConstVector(val, sizes...));
}

// Helper: returns c.size(), c.front().size(), ... (k steps). Useful to get
// e.g. the size of a matrix.
//
// Result type is an n-tuple of int.
template <size_t n, class Cont>
NTuple<n, int> Sizes(const Cont& cont) {
    if constexpr (n == 0) {
        return std::tuple<>();
    } else {
        return std::tuple_cat(std::make_tuple((int)cont.size()),
                              Sizes<n - 1>(cont.front()));
    }
}

size_t CombineHash(size_t h, size_t val) {
    return h ^ ((h * 999983) + 997391 + val);
}

size_t SeqHash() {
    return 0;
}

template <typename T, typename... Ts>
size_t SeqHash(const T& val, const Ts&... vals) {
    return CombineHash(SeqHash(vals...), std::hash<T>()(val));
}

template <typename Tuple, std::size_t... Is>
size_t TupleHashImpl(const Tuple& t, std::index_sequence<Is...>) {
    return SeqHash(std::get<Is>(t)...);
}

template <typename... Ts>
size_t TupleHash(const std::tuple<Ts...>& t) {
    return TupleHashImpl(t, std::index_sequence_for<Ts...>());
}

// Hasher for tuples to be used with std::unordered_set and std::unordered_map.
//
// Example: std::unordered_set<NTuple<3, int>, TupleHasher> set;
struct TupleHasher {
    template <typename... Ts>
    size_t operator()(const std::tuple<Ts...>& t) const {
        return TupleHash(t);
    }
};

#endif