#ifndef __AOC_LEX_H__
#define __AOC_LEX_H__

#include <compare>

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