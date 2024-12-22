#include <cassert>
#include <cmath>
#include <iostream>
#include <tuple>

#include "collections.h"
#include "numbers.h"
#include "order.h"
#include "parse.h"

template <typename F>
constexpr long double kEpsilon;
template <>
constexpr long double kEpsilon<float> = 1e-4;
template <>
constexpr long double kEpsilon<double> = 1e-13;
template <>
constexpr long double kEpsilon<long double> = 0;

template <typename T>
bool Near(Rational<T> r, long double v) {
    long double rd = static_cast<long double>(r.Num()) /
                     static_cast<long double>(r.Denom());
    return abs(rd - v) <= 1e-16;
}

template <typename F>
bool Near(F f, long double v)
    requires std::floating_point<F>
{
    return abs((long double)f - v) <= kEpsilon<F>;
}

template <typename R, typename I, typename F>
void TestRational(
    const std::string& rational_type,
    const std::string& integral_type,
    const std::string& floating_type) {
    std::cerr << "Testing " << rational_type << " with " << integral_type << " and " << floating_type << "..." << std::endl;

    assert(R(42).Num() == 42);
    assert(R(42).Denom() == 1);
    assert((R(1) / R(42)).Num() == 1);
    assert((R(1) / R(42)).Denom() == 42);
    assert(R(1) / (R(1) / (R)42) == (R)42);

    for (I n1 = -10; n1 <= 10; n1++) {
        for (I d1 = -10; d1 <= 10; d1++) {
            if (d1 == 0) {
                continue;
            }

            R ra = (R)n1 / d1;
            long double da = (long double)n1 / d1;
            I ia = n1 / d1;
            F fa = (F)n1 / d1;

            assert((long double)-ra == -da);
            assert((long double)floor(ra) == floor(da));
            assert((long double)ceil(ra) == ceil(da));
            assert((long double)trunc(ra) == trunc(da));
            assert((long double)round(ra) == round(da));
            assert((long double)abs(ra) == abs(da));
            static_assert(std::is_same_v<decltype(-ra), R>);
            static_assert(std::is_same_v<decltype(floor(ra)), R>);
            static_assert(std::is_same_v<decltype(ceil(ra)), R>);
            static_assert(std::is_same_v<decltype(trunc(ra)), R>);
            static_assert(std::is_same_v<decltype(round(ra)), R>);
            static_assert(std::is_same_v<decltype(abs(ra)), R>);

            assert((I)ra == (I)da);
            assert((F)ra == (F)da);

            for (I n2 = -10; n2 <= 10; n2++) {
                for (I d2 = -10; d2 <= 10; d2++) {
                    if (d2 == 0) {
                        continue;
                    }

                    R rb = (R)n2 / d2;
                    long double db = (long double)n2 / d2;
                    I ib = n2 / d2;
                    F fb = (F)n2 / d2;

                    assert((ra == rb) == (da == db));
                    assert((ra == ib) == (da == ib));
                    assert((ia == rb) == (ia == db));
                    static_assert(std::is_same_v<decltype(ra == rb), bool>);
                    static_assert(std::is_same_v<decltype(ra == ib), bool>);
                    static_assert(std::is_same_v<decltype(ia == rb), bool>);

                    assert((ra <=> rb) == (da <=> db));
                    assert((ra <=> ib) == (da <=> ib));
                    assert((ia <=> rb) == (ia <=> db));
                    static_assert(std::is_same_v<decltype(ra <=> rb), std::strong_ordering>);
                    static_assert(std::is_same_v<decltype(ra <=> ib), std::strong_ordering>);
                    static_assert(std::is_same_v<decltype(ia <=> rb), std::strong_ordering>);

                    // Comparisons between rationals and floats are tested only
                    // on on power-of-2 denominators, when exact comparisons
                    // of floats are reliable.
                    if (d1 & (d1 - 1) == 0 && d2 & (d2 - 1) == 0) {
                        assert((ra == fb) == (da == fb));
                        assert((fa == rb) == (fa == db));
                        assert((ra <=> fb) == (da <=> fb));
                        assert((fa <=> rb) == (fa <=> db));
                    }
                    static_assert(std::is_same_v<decltype(ra == fb), bool>);
                    static_assert(std::is_same_v<decltype(fa == rb), bool>);
                    static_assert(std::is_same_v<decltype(ra <=> fb), decltype(ra <=> db)>);
                    static_assert(std::is_same_v<decltype(fa <=> rb), decltype(fa <=> db)>);

                    assert(Near(ra + rb, da + db));
                    assert(Near(ra + ib, da + ib));
                    assert(Near(ra + fb, da + fb));
                    assert(Near(ia + rb, ia + db));
                    assert(Near(fa + rb, fa + db));
                    static_assert(std::is_same_v<decltype(ra + rb), R>);
                    static_assert(std::is_same_v<decltype(ra + ib), R>);
                    static_assert(std::is_same_v<decltype(ra + fb), F>);
                    static_assert(std::is_same_v<decltype(ia + rb), R>);
                    static_assert(std::is_same_v<decltype(fa + rb), F>);

                    assert(Near(ra - rb, da - db));
                    assert(Near(ra - ib, da - ib));
                    assert(Near(ra - fb, da - fb));
                    assert(Near(ia - rb, ia - db));
                    assert(Near(fa - rb, fa - db));
                    static_assert(std::is_same_v<decltype(ra - rb), R>);
                    static_assert(std::is_same_v<decltype(ra - ib), R>);
                    static_assert(std::is_same_v<decltype(ra - fb), F>);
                    static_assert(std::is_same_v<decltype(ia - rb), R>);
                    static_assert(std::is_same_v<decltype(fa - rb), F>);

                    assert(Near(ra * rb, da * db));
                    assert(Near(ra * ib, da * ib));
                    assert(Near(ra * fb, da * fb));
                    assert(Near(ia * rb, ia * db));
                    assert(Near(fa * rb, fa * db));
                    static_assert(std::is_same_v<decltype(ra * rb), R>);
                    static_assert(std::is_same_v<decltype(ra * ib), R>);
                    static_assert(std::is_same_v<decltype(ra * fb), F>);
                    static_assert(std::is_same_v<decltype(ia * rb), R>);
                    static_assert(std::is_same_v<decltype(fa * rb), F>);

                    if (n2 != 0) assert(Near(ra / rb, da / db));
                    if (ib != 0) assert(Near(ra / ib, da / ib));
                    if (fb != 0) assert(Near(ra / fb, da / fb));
                    if (n2 != 0) assert(Near(ia / rb, ia / db));
                    if (n2 != 0) assert(Near(fa / rb, fa / db));
                    static_assert(std::is_same_v<decltype(ra / rb), R>);
                    static_assert(std::is_same_v<decltype(ra / ib), R>);
                    static_assert(std::is_same_v<decltype(ra / fb), F>);
                    static_assert(std::is_same_v<decltype(ia / rb), R>);
                    static_assert(std::is_same_v<decltype(fa / rb), F>);

                    {
                        R t = ra;
                        t += rb;
                        assert(Near(t, da + db));
                    }
                    {
                        R t = ra;
                        t += ib;
                        assert(Near(t, da + ib));
                    }

                    {
                        R t = ra;
                        t -= rb;
                        assert(Near(t, da - db));
                    }
                    {
                        R t = ra;
                        t -= ib;
                        assert(Near(t, da - ib));
                    }

                    {
                        R t = ra;
                        t *= rb;
                        assert(Near(t, da * db));
                    }
                    {
                        R t = ra;
                        t *= ib;
                        assert(Near(t, da * ib));
                    }

                    if (n2 != 0) {
                        R t = ra;
                        t /= rb;
                        assert(Near(t, da / db));
                    }
                    if (ib != 0) {
                        R t = ra;
                        t /= ib;
                        assert(Near(t, da / ib));
                    }
                }
            }
        }
    }

    // Add some numbers to make sure there's no overflow.
    R acc = 0;
    for (I i = 0; i < 1000; i++) {
        acc += (R)i / 1000;
    }
    assert(acc == (R)999 / 2);

    // Subtract some numbers to make sure there's no overflow.
    acc = 0;
    for (I i = 0; i < 1000; i++) {
        acc -= (R)i / 1000;
    }
    assert(acc == (R)-999 / 2);

    // Multiply some numbers to make sure there's no overflow.
    acc = 1;
    for (I i = 1; i < 1000; i++) {
        acc *= ((R)(i + 1) / i);
    }
    assert(acc == 1000);

    // Divide some numbers to make sure there's no overflow.
    acc = 1;
    for (I i = 1; i < 1000; i++) {
        acc /= ((R)(i + 1) / i);
    }
    assert(acc == (R)1 / 1000);
}

int main() {
    std::cerr << "Testing Gcd() and Euclid()..." << std::endl;
    for (int i = -100; i < 100; i++) {
        for (int j = -100; j < 100; j++) {
            int d = Gcd(i, j), p, q;
            std::tie(p, q) = Euclid(i, j);
            assert(p * i + q * j == d);
            if (i != 0 || j != 0) {
                assert(d > 0);
                assert(i % d == 0);
                assert(j % d == 0);
            }
        }
    }
    assert(Gcd(0, 0) == 0);

    std::cerr << "Testing Inverse()..." << std::endl;
    {
        const int p = 107;
        for (int i = -p + 1; i < p; i++) {
            if (i == 0) {
                continue;
            }
            assert(((i + p) * Inverse(i, p)) % p == 1);
        }
    }
    assert(Inverse(1, 0) == 1);
    assert(Inverse(-1, 0) == -1);
    assert(Inverse(37, 1) == 0);

    std::cerr << "Testing GetPrimes()..." << std::endl;
    assert((GetPrimes(20) == std::vector<int>{2, 3, 5, 7, 11, 13, 17, 19}));

    std::cerr << "Testing rounding divisions..." << std::endl;
    for (int i = -24; i <= 24; i++) {
        assert(FloorDiv(i, 10) == floor((double)i / 10));
        assert(CeilDiv(i, 10) == ceil((double)i / 10));
        assert(TruncDiv(i, 10) == trunc((double)i / 10));
        assert(RoundDiv(i, 10) == round((double)i / 10));
    }

    TestRational<Rat, int, float>("Rat", "int", "float");
    TestRational<LRat, long, double>("LRat", "long", "double");
    TestRational<LLRat, long long, long double>("LLRat", "long long", "long double");
    TestRational<Rat, long long, double>("Rat", "long long", "double");
    TestRational<LLRat, int, double>("LLRat", "int", "double");

    std::cerr << "Testing Rational conversions..." << std::endl;
    assert((LLRat)((Rat)2 / 3) == (LLRat)2 / 3);
    assert((Rat)((LLRat)2 / 3) == (Rat)2 / 3);

    std::cerr << "Testing NTuple..." << std::endl;
    static_assert(std::is_same_v<NTuple<0, int>, std::tuple<>>);
    static_assert(std::is_same_v<NTuple<1, int>, std::tuple<int>>);
    static_assert(std::is_same_v<NTuple<2, int>, std::tuple<int, int>>);
    static_assert(std::is_same_v<NTuple<3, char>, std::tuple<char, char, char>>);

    std::cerr << "Testing Find..." << std::endl;
    assert(Find<2>(std::vector<std::string>{"abcdef", "gijklmnop"}, 'l') == std::make_tuple(1, 4));
    assert(Find<2>(std::vector<std::string>{"abcdef", "gijklmnop"}, 'z') == std::nullopt);
    assert(FindOrDie<2>(std::vector<std::string>{"abcdef", "gijklmnop"}, 'd') == std::make_tuple(0, 3));

    std::cerr << "Testing NestedVector..." << std::endl;
    static_assert(std::is_same_v<NestedVector<0, int>, int>);
    static_assert(std::is_same_v<NestedVector<1, int>, std::vector<int>>);
    static_assert(std::is_same_v<NestedVector<2, int>, std::vector<std::vector<int>>>);
    static_assert(std::is_same_v<NestedVector<3, NestedVector<4, std::string>>,
        NestedVector<7, std::string>>);

    std::cerr << "Testing ConstVector..." << std::endl;
    assert((ConstVector(42, 2, 3) == NestedVector<2, int>{{42, 42, 42}, {42, 42, 42}}));
    assert(Sizes<3>(ConstVector('x', 3, 4, 5)) == std::make_tuple(3, 4, 5));

    std::cerr << "OK" << std::endl;
    std::cout << SeqHash(1, 2, 3, 4) << std::endl;
    std::cout << TupleHash(std::make_tuple()) << std::endl;
    std::cout << std::hash<int>()(42) << std::endl;
    return 0;
}