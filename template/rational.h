#ifndef __AOC_RATIONAL_H__
#define __AOC_RATIONAL_H__

#include <cassert>
#include <compare>
#include <ostream>

#include "numbers.h"

// Rational numbers. T is the type of numerator & denominator.
template<typename T>
class Rational {
public:
    Rational() : n_(0), d_(1) {}
    Rational(T x) : n_(x), d_(1) {}

    Rational(const Rational&) = default;
    Rational& operator=(const Rational&) = default;
    Rational(Rational&&) = default;
    Rational& operator=(Rational&&) = default;

    Rational Normal() const {
        if (d_ == 0) {
            return *this;
        }
        T x = Gcd(n_, d_) * ((d_ < 0) ? -1 : 1);
        return Rational{n_ / x, d_ / x};
    }

    friend std::ostream& operator<<(std::ostream& out, const Rational& r) {
        Rational s = r.Normal();
        if (s.d_ == 1) {
            return out << s.n_;
         }
         return out << s.n_ << '/' << s.d_;
    }

    bool operator==(const Rational& other) const {
        return n_ * other.d_ == other.n_ * d_;
    }
    bool operator==(T other) const {
        return *this == Rational(other);
    }
    friend bool operator==(T other, const Rational& r) {
        return Rational(other) == r;
    }

    std::strong_ordering operator<=>(const Rational& other) const {
        Rational x = *this - other;
        return (x.d_ > 0) ? (x.n_ <=> 0) : (0 <=> x.n_);
    }

    Rational operator+(const Rational& other) const {
        return Rational(n_ * other.d_ + other.n_ * d_, d_ * other.d_);
    }
    Rational operator+(T other) const {
        return *this + Rational(other);
    }
    friend Rational operator+(T other, const Rational& r) {
        return Rational(other) + r;
    }

    Rational operator-() const {
        return Rational(-n_, d_);
    }

    Rational operator-(const Rational& other) const {
        return Rational(n_ * other.d_ - other.n_ * d_, d_ * other.d_);
    }
    Rational operator-(T other) const {
        return *this - Rational(other);
    }
    friend Rational operator-(T other, const Rational& r) {
        return Rational(other) - r;
    }

    Rational operator*(const Rational& other) const {
        return Rational(n_ * other.n_, d_ * other.d_);
    }
    Rational operator*(T other) const {
        return *this * Rational(other);
    }
    friend Rational operator*(T other, const Rational& r) {
        return Rational(other) * r;
    }

    Rational operator/(const Rational& other) const {
        return Rational(n_ * other.d_, other.n_ * d_);
    }
    Rational operator/(T other) const {
        return *this / Rational(other);
    }
    friend Rational operator/(T other, const Rational& r) {
        return Rational(other) / r;
    }

    explicit operator float() const {
        return static_cast<float>(n_) / static_cast<float>(d_);
    }
    explicit operator double() const {
        return static_cast<double>(n_) / static_cast<double>(d_);
    }

    T Floor() const {
        return (d_ >= 0) ? (n_ / d_) : ((-n_) / (-d_));
    }

    T Ceil() const {
        return (d_ >= 0) ? ((n_ + d_ - 1) / d_) : ((-n_ - d_ - 1) / (-d_));
    }

private:
    Rational(T num, T denom) : n_(num), d_(denom) {}

    T n_;
    T d_;
};

using Rat = Rational<int>;
using LRat = Rational<long>;
using LLRat = Rational<long long>;

#endif