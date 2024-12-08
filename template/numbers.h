#ifndef __AOC_NUMBERS_H__
#define __AOC_NUMBERS_H__

#include <cassert>
#include <compare>
#include <concepts>
#include <ostream>
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

// Multiplicative inverse element in Z/mZ. The result is normalized to be
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
    if (m != 0) {
        y %= m;
        if (y < 0) {
            y += m;
        }
    }
    return y;
}

// Divisions with different sorts of rounding.

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

// Divide and round towards -infinity.
template<typename T>
T FloorDiv(T a, T b) {
    if (b < 0) {
        a = -a;
        b = -b;
    }
    return (a >= 0) ? (a / b) : ((a - b + 1) / b);
}

// Divide and round towards +infinity.
template<typename T>
T CeilDiv(T a, T b) {
    if (b < 0) {
        a = -a;
        b = -b;
    }
    return (a >= 0) ? ((a + b - 1) / b) : (a / b);
}

// Divide and rounds towards 0.
template<typename T>
T TruncDiv(T a, T b) {
    return a / b;
}

// Divide and round to nearest integer, rounding half-integers away from 0.
template<typename T>
T RoundDiv(T a, T b) {
    if (b < 0) {
        a = -a;
        b = -b;
    }
    return (a >= 0) ? ((2 * a + b) / (2 * b)) : ((2 * a - b) / (2 * b));
}

// Rational numbers. T is the type of numerator and denominator.
template<typename T>
class Rational {
public:
    Rational() : n_(0), d_(1) {}

    template<class U>
    Rational(U x) requires std::integral<U> : n_(static_cast<T>(x)), d_(1) {}

    Rational(const Rational&) = default;
    Rational& operator=(const Rational&) = default;
    Rational(Rational&&) = default;
    Rational& operator=(Rational&&) = default;

    template<typename U>
    explicit Rational(const Rational<U>& other) :
        n_(static_cast<T>(other.Num())), d_(static_cast<T>(other.Denom())) {}

    T Num() const {
        return n_;
    }

    T Denom() const {
        return d_;
    }

    Rational Normal() const {
        T x = Gcd(n_, d_) * ((d_ < 0) ? -1 : 1);
        return Rational{n_ / x, d_ / x};
    }

    friend std::ostream& operator<<(std::ostream& out, const Rational& r) {
        if (r.d_ == 1) {
            return out << r.n_;
        }
        return out << r.n_ << '/' << r.d_;
    }


    // Equality comparison.

    bool operator==(const Rational& other) const {
        // This is a "terminating" operation, i.e. returning a non-Rational.
        // If we got a bad rational with zero denominator, probably due to
        // arithmetic overflow, complain now before the error propagates
        // further.
        assert (d_ != 0 && other.d_ != 0);
        return n_ * other.d_ == d_ * other.n_;
    }

    template<typename U>
    bool operator==(U other) const requires std::integral<U> {
        return *this == Rational(other);
    }

    template<typename U>
    bool operator==(U) const = delete;


    // Order comparison.

    std::strong_ordering operator<=>(const Rational& other) const {
        Rational x = *this - other;

        // This is a "terminating" operation (doesn't return Rational), so we
        // check now if Rational computations produced an invalid value.
        assert (x.d_ != 0);
        return (x.d_ > 0) ? (x.n_ <=> 0) : (0 <=> x.n_);
    }

    template<typename U>
    std::strong_ordering operator<=>(U other) const requires std::integral<U> {
        return *this <=> Rational(other);
    }

    template<typename U>
    std::strong_ordering operator<=>(U) const = delete;


    // Addition.

    Rational operator+(const Rational& other) const {
        return Rational(n_ * other.d_ + other.n_ * d_, d_ * other.d_).Normal();
    }

    template<typename U>
    Rational operator+(U other) const requires std::integral<U> {
        return *this + Rational(other);
    }

    template<typename U>
    Rational operator+(U) const = delete;

    template<typename U>
    friend Rational operator+(U other, const Rational& r) requires std::integral<U> {
        return Rational(other) + r;
    }

    template<typename U>
    friend Rational operator+(U, const Rational&) = delete;


    // Unary minus.

    Rational operator-() const {
        return Rational(-n_, d_);
    }


    // Subtraction.

    Rational operator-(const Rational& other) const {
        return Rational(n_ * other.d_ - other.n_ * d_, d_ * other.d_).Normal();
    }

    template<typename U>
    Rational operator-(U other) const requires std::integral<U> {
        return *this - Rational(other);
    }

    template<typename U>
    Rational operator-(U) const = delete;
    
    template<typename U>
    friend Rational operator-(U other, const Rational& r) requires std::integral<U> {
        return Rational(other) - r;
    }

    template<typename U>
    friend Rational operator-(U, const Rational&) = delete;


    // Multiplication.

    Rational operator*(const Rational& other) const {
        return Rational(n_ * other.n_, d_ * other.d_).Normal();
    }

    template<typename U>
    Rational operator*(U other) const requires std::integral<U> {
        return *this * Rational(other);
    }

    template<typename U>
    Rational operator*(U) const = delete;

    template<typename U>
    friend Rational operator*(U other, const Rational& r) requires std::integral<U> {
        return Rational(other) * r;
    }

    template<typename U>
    friend Rational operator*(U, const Rational&) = delete;


    // Division.

    Rational operator/(const Rational& other) const {
        assert (other.n_ != 0);
        return Rational(n_ * other.d_, other.n_ * d_).Normal();
    }

    template<typename U>
    Rational operator/(U other) const requires std::integral<U> {
        return *this / Rational(other);
    }

    template <typename U>
    Rational operator/(U) const = delete;

    template<typename U>
    friend Rational operator/(U other, const Rational& r) requires std::integral<U> {
        return Rational(other) / r;
    }

    template <typename U>
    friend Rational operator/(U, const Rational&) = delete;    


    // Compound assignment by sum.

    Rational& operator+=(const Rational& other) {
        *this = *this + other;
        return *this;
    }

    template<typename U>
    Rational& operator+=(U other) requires std::integral<U> {
        return (*this += Rational(other));
    }

    template<typename U>
    Rational& operator+=(U) = delete;


    // Compound assignment by difference.

    Rational& operator-=(const Rational& other) {
        *this = *this - other;
        return *this;
    }

    template<typename U>
    Rational& operator-=(U other) requires std::integral<U> {
        return (*this -= Rational(other));
    }

    template<typename U>
    Rational& operator-=(U) = delete;


    // Compound assignment by product.

    Rational& operator*=(const Rational& other) {
        *this = *this * other;
        return *this;
    }

    template<typename U>
    Rational& operator*=(U other) requires std::integral<U> {
        return (*this *= Rational(other));
    }

    template<typename U>
    Rational& operator*=(U) = delete;


    // Compound assignment by quotient.

    Rational& operator/=(const Rational& other) {
        *this = *this / other;
        return *this;
    }

    template<typename U>
    Rational& operator/=(U other) requires std::integral<U> {
        return (*this /= Rational(other));
    }

    template<typename U>
    Rational& operator/=(U) = delete;


    // TODO: add operations with floating point numbers, resulting in floating point numbers.


    // Cast to primitive types.

    template<class U>
    operator U() const requires std::integral<U>{
        return static_cast<U>(n_ / d_);
    }

    template<class U>
    operator U() const requires std::floating_point<U> {
        return static_cast<U>(n_) / static_cast<U>(d_); 
    }


    // Rounding.

    Rational Floor() const {
        return FloorDiv<T>(n_, d_);
    }

    Rational Ceil() const {
        return CeilDiv<T>(n_, d_);
    }

    Rational Trunc() const {
        return TruncDiv<T>(n_, d_);
    }

    Rational Round() const {
        return RoundDiv<T>(n_, d_);
    }

    Rational Abs() const {
        return Rational(std::abs(n_), std::abs(d_));
    }

private:
    Rational(T num, T denom) : n_(num), d_(denom) {}

    T n_;
    T d_;
};

template<typename T>
Rational<T> floor(const Rational<T>& r) {
    return r.Floor();
}

template<typename T>
Rational<T> ceil(const Rational<T>& r) {
    return r.Ceil();
}

template<typename T>
Rational<T> trunc(const Rational<T>& r) {
    return r.Trunc();
}

template<typename T>
Rational<T> round(const Rational<T>& r) {
    return r.Round();
}

template<typename T>
Rational<T> abs(const Rational<T>& r) {
    return r.Abs();
}

using Rat = Rational<int>;
using LRat = Rational<long>;
using LLRat = Rational<long long>;

#endif