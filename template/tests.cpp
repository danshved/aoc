#include <cassert>
#include <iostream>
#include <tuple>
#include <cmath>

#include "numbers.h"
#include "order.h"

template<typename T>
bool Near(Rational<T> r, double v) {
    return abs((double)r.Num() / r.Denom() - v) < 1e-12;
}

template <typename R, typename T>
void TestRational(
        const std::string& rational_type,
        const std::string& integral_type)
{
    std::cerr << "Testing " << rational_type <<
            " with " << integral_type << "..." << std::endl;

    assert(R(42).Num() == 42);
    assert(R(42).Denom() == 1);
    assert((R(1) / R(42)).Num() == 1);
    assert((R(1) / R(42)).Denom() == 42);
    assert(R(1) / (R(1) / (R)42) == (R)42);

    for (T n1 = -10; n1 <= 10; n1++) {
        for (T d1 = -10; d1 <= 10; d1++) {
            if (d1 == 0) {
                continue;
            }

            R ra = (R)n1 / d1;
            double da = (double)n1 / d1;
            T ta = n1 / d1;

            assert(Near(-ra, -da));
            assert((int)ra == (int)da);
            assert((long)ra == (long)da);
            assert((long long)ra == (long long)da);
            assert((float)ra == (float)da);
            assert((double)ra == da);
            assert((double)floor(ra) == floor(da));
            assert((double)ceil(ra) == ceil(da));
            assert((double)trunc(ra) == trunc(da));
            assert((double)round(ra) == round(da));
            assert((double)abs(ra) == abs(da));

            static_assert(std::is_same_v<decltype(floor(ra)), R>);
            static_assert(std::is_same_v<decltype(ceil(ra)), R>);
            static_assert(std::is_same_v<decltype(trunc(ra)), R>);
            static_assert(std::is_same_v<decltype(round(ra)), R>);
            static_assert(std::is_same_v<decltype(abs(ra)), R>);

            for (T n2 = -10; n2 <= 10; n2++) {
                for (T d2 = -10; d2 <= 10; d2++) {
                    if (d2 == 0) {
                        continue;
                    }

                    R rb = (R)n2 / d2;
                    double db = (double)n2 / d2;
                    T ib = n2 / d2;

                    assert((ra == rb) == (da == db));
                    assert((ra == ib) == (da == ib));
                    assert((ta == rb) == (ta == db));

                    assert((ra <=> rb) == (da <=> db));
                    assert((ra <=> ib) == (da <=> ib));
                    assert((ta <=> rb) == (ta <=> db));

                    assert(Near(ra + rb, da + db));
                    assert(Near(ra + ib, da + ib));
                    assert(Near(ta + rb, ta + db));

                    assert(Near(ra - rb, da - db));
                    assert(Near(ra - ib, da - ib));
                    assert(Near(ta - rb, ta - db));

                    assert(Near(ra * rb, da * db));
                    assert(Near(ra * ib, da * ib));
                    assert(Near(ta * rb, ta * db));

                    if (n2 != 0) assert(Near(ra / rb, da / db));
                    if (ib != 0) assert(Near(ra / ib, da / ib));
                    if (n2 != 0) assert(Near(ta / rb, ta / db));

                    { R t = ra; t += rb; assert(Near(t, da + db)); }
                    { R t = ra; t += ib; assert(Near(t, da + ib)); }

                    { R t = ra; t -= rb; assert(Near(t, da - db)); }
                    { R t = ra; t -= ib; assert(Near(t, da - ib)); }    

                    { R t = ra; t *= rb; assert(Near(t, da * db)); }
                    { R t = ra; t *= ib; assert(Near(t, da * ib)); }   

                    if (n2 != 0) { R t = ra; t /= rb; assert(Near(t, da / db)); }
                    if (ib != 0) { R t = ra; t /= ib; assert(Near(t, da / ib)); }     
                 }
            }
        }
    }

    // Add some numbers to make sure there's no overflow.
    R acc = 0;
    for (T i = 0; i < 1000; i++) {
        acc += (R)i / 1000;
    }
    assert (acc == (R)999 / 2);

    // Subtract some numbers to make sure there's no overflow.
    acc = 0;
    for (T i = 0; i < 1000; i++) {
        acc -= (R)i / 1000;
    }
    assert (acc == (R)-999 / 2);

    // Multiply some numbers to make sure there's no overflow.
    acc = 1;
    for (T i = 1; i < 1000; i++) {
        acc *= ((R)(i + 1) / i);
    }
    assert (acc == 1000);

    // Divide some numbers to make sure there's no overflow.
    acc = 1;
    for (T i = 1; i < 1000; i++) {
        acc /= ((R)(i + 1) / i);
    }
    assert (acc == (R)1 / 1000);
}

int main() {
    std::cerr << "Testing Gcd() and Euclid()..." << std::endl;
    for (int i = -100; i < 100; i++) {
        for (int j = -100; j < 100; j++) {
            int d = Gcd(i, j), p, q;
            std::tie(p, q) = Euclid(i, j);
            assert (p * i + q * j == d);
            if (i != 0 || j != 0) {
                assert (d > 0);
                assert (i % d == 0);
                assert (j % d == 0);
            }
        }
    }
    assert (Gcd(0, 0) == 0);

    std::cerr << "Testing Inverse()..." << std::endl;
    {
        const int p = 107;
        for (int i = -p + 1; i < p; i++) {
            if (i == 0) {
                continue;
            }
            assert (((i + p) * Inverse(i, p)) % p == 1);
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

    TestRational<Rat, int>("Rat", "int");
    TestRational<LRat, long>("LRat", "long");
    TestRational<LLRat, long long>("LLRat", "long long");
    TestRational<Rat, long long>("Rat", "long long");
    TestRational<LLRat, int>("LLRat", "int");

    std::cerr << "Testing Rational conversions..." << std::endl;
    assert((LLRat)((Rat)2 / 3) == (LLRat)2 / 3);
    assert((Rat)((LLRat)2 / 3) == (Rat)2 / 3);
    
    std::cerr << "OK" << std::endl;
    return 0;
}