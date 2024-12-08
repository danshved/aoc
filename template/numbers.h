#ifndef __AOC_NUMBERS_H__
#define __AOC_NUMBERS_H__

#include <cassert>
#include <utility>
#include <vector>

// Greatest common divisor.
template<typename T>
T Gcd(T a, T b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        a %= b;
        std::swap(a, b);
    }
    return a;
}

// Extended Euclid's algorithm.
// Returns such (x, y) that a * x + b * y = Gcd(a, b).
template<typename T>
std::pair<T, T> Euclid(T a, T b) {
    T xa = 1, ya = 0, xb = 0, yb = 1;
    if (a < 0) {
       std::tie(a, xa, ya) = std::make_tuple(-a, -xa, -ya);
    }
    if (b < 0) {
       std::tie(b, xb, yb) = std::make_tuple(-b, -xb, -yb);
    }
    while (b != 0) {
        T k = a / b;
        std::tie(a, b) = std::make_pair(b, a - k * b);
        std::tie(xa, xb) = std::make_pair(xb, xa - k * xb);
        std::tie(ya, yb) = std::make_pair(yb, ya - k * yb);
    }
    return std::make_pair(xa, ya);
}

// Multiplicative inverse element in Z_m. The result is normalized to be
// within [0, abs(m)).
//
// Inverse (x, m) * x == 1  (mod m).
//
// Dies if x and m are not coprime.
template<typename T>
T Inverse(T x, T m) {
    if (m < 0) {
        m = -m;
    }

    T y, n;
    std::tie(y, n) = Euclid(x, m);
    assert (x * y + m * n == 1);
    y %= m;
    if (y < 0) {
        y += m;
    }
    return y;
}

// Generates all primes that are less than bound, in ascending order.
// Time: O(bound * log log bound).
template<typename T>
std::vector<T> GetPrimes(T bound) {
    std::vector<bool> has_divisor(bound, false);
    std::vector<T> result;

    for (T i = 2; i < bound; i++) {
        if (has_divisor[i]) {
            continue;
        }
        result.push_back(i);
        for (T j = 2 * i; j < bound; j += i) {
            has_divisor[j] = true;
        }
    }

    return result;
}

#endif