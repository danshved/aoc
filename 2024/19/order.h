#ifndef __AOC_ORDER_H__
#define __AOC_ORDER_H__

#include <compare>

// Sign of a number, or of any comparison result.
template<typename T>
int Sign(T x) {
    auto c = x <=> 0;
    return (c > 0) ? 1 : (c < 0) ? -1 : 0;
}

// Chains comparison results lexicographically, i.e. returns the first one
// that is not "equivalent".
//
// The output type is the weakest comparison category of all the input types.
//
// Lex(a <=> x, b <=> y, c <=> z)  ==  std::tie(a, b, c) <=> std::tie(x, y, z).
std::strong_ordering Lex() {
    return std::strong_ordering::equivalent;
}

template<typename T, typename... Ts>
auto Lex(T x, Ts... xs) {
    return (x != 0) ? x : Lex(xs...);
}

#endif