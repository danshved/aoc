#ifndef __AOC_PARSE_H__
#define __AOC_PARSE_H__

#include <algorithm>
#include <cassert>
#include <cctype>
#include <fstream>
#include <functional>
#include <iterator>
#include <ranges>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

// Parse vector of numbers separated by spaces and/or commas.
template <typename T>
std::vector<T> ParseVector(std::string s) {
    for (char& c : s) {
        if (c == ',') {
            c = ' ';
        }
    }

    std::istringstream iss(s);
    std::vector<T> result;
    std::copy(std::istream_iterator<T>(iss), std::istream_iterator<T>(),
              std::back_inserter(result));
    return result;
}

// Format vector for debugging.
template <typename T>
std::string FormatVector(const std::vector<T>& v, std::string delim = " ") {
    std::ostringstream oss;
    for (auto it = v.begin(); it != v.end(); ++it) {
        if (it != v.begin()) {
            oss << delim;
        }
        oss << *it;
    }
    return oss.str();
}

// Splits a container using the given delimiter. The resulting vector's size
// is always one plus the largest number of non-overlapping delimiters in the
// input.
//
// If the delimiter never occurs, the result is a size-1 vector containing the
// input, even if the input is empty.
template <typename Cont>
std::vector<Cont> Split(const Cont& xs, const Cont& delim) {
    auto delim_size = std::ranges::distance(std::begin(delim), std::end(delim));
    assert(delim_size > 0);

    std::vector<Cont> result;
    const auto searcher = std::boyer_moore_searcher(std::begin(delim), std::end(delim));
    auto cur = xs.begin(), end = xs.end();
    while (true) {
        auto it = std::search(cur, end, searcher);
        result.emplace_back(cur, it);
        if (it == end) {
            break;
        } else {
            cur = it;
            std::ranges::advance(cur, delim_size);
        }
    }
    return result;
}

// A more convenient overload of Split() for strings.
std::vector<std::string> Split(const std::string& s, const std::string& delim) {
    return Split<std::string>(s, delim);
}

template <typename Cont, typename I>
auto SplitNImpl(I begin, I end) {
    return std::make_tuple(Cont(begin, end));
}

template <typename Cont, typename I, typename... Ts>
auto SplitNImpl(I begin, I end, const Cont& delim, const Ts&... delims) {
    auto delim_size = std::ranges::distance(std::begin(delim), std::end(delim));
    assert(delim_size > 0);

    I it = std::search(begin, end, std::boyer_moore_searcher(std::begin(delim), std::end(delim)));
    assert(it != end);

    I next = it;
    std::ranges::advance(next, delim_size);
    return std::tuple_cat(std::make_tuple(Cont(begin, it)),
                          SplitNImpl<Cont, I>(next, end, delims...));
}

template <typename Cont>
struct SplitNHelper {
    template <typename... Ts>
    static auto SplitN(const Cont& xs, const Ts&... delims) {
        return SplitNImpl(xs.begin(), xs.end(), delims...);
    }

    template <typename I>
    static auto SplitNImpl(I begin, I end) {
        return std::make_tuple(Cont(begin, end));
    }

    template <typename I, typename... Ts>
    static auto SplitNImpl(I begin, I end, const Cont& delim, const Ts&... delims) {
        auto delim_size = std::ranges::distance(std::begin(delim), std::end(delim));
        assert(delim_size > 0);

        I it = std::search(begin, end, std::boyer_moore_searcher(std::begin(delim), std::end(delim)));
        assert(it != end);

        I next = it;
        std::ranges::advance(next, delim_size);
        return std::tuple_cat(std::make_tuple(Cont(begin, it)),
                              SplitNImpl<I>(next, end, delims...));
    }
};

template <typename Cont, typename... Ts>
auto SplitN(const Cont& xs, const Ts&... delims) {
    return SplitNHelper<Cont>::SplitN(xs, delims...);
}

template <std::convertible_to<std::string> Cont, typename... Ts>
auto SplitN(const Cont& xs, const Ts&... delims) {
    return SplitNHelper<std::string>::SplitN(xs, delims...);
}

// Convenience overload that allows doing things with brace initializers like this:
//
// std::vector<string> lines = {"a", "b", "", "c", "d"};
// Split2(lines, {""});  //returns tuple of {"a", "b"} and {"c", "d"}.
template <typename Cont>
std::tuple<Cont, Cont> Split2(const Cont& xs, const Cont& delim) {
    return SplitN(xs, delim);
}

// Removes whitespace in the beginning and end of a string.
std::string Trim(const std::string& s) {
    int i = 0, j = s.length();
    for (; i < j && std::isspace(s[i]); i++) {
    };
    for (; i < j && std::isspace(s[j - 1]); j--) {
    };
    return s.substr(i, j - i);
}

// Reads entire text file into a string. Newlines in the output are plain '\n'.
std::string GetContents(const std::string& filename) {
    std::ifstream f(filename);
    std::stringstream buffer;
    buffer << f.rdbuf();
    return buffer.str();
}

#endif