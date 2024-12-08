#include <cassert>
#include <iostream>
#include <tuple>
#include <cmath>

#include "numbers.h"
#include "order.h"

template<typename T>
bool RatNear(Rational<T> r, double v) {
    return abs((double)r.Num() / r.Denom() - v) < 1e-12;
}

template <typename T>
void TestRational(const std::string& type_name) {
    using R = Rational<T>;
    std::cerr << "Testing " << type_name << "..." << std::endl;

    assert(R(42).Num() == 42);
    assert(R(42).Denom() == 1);
    assert((R(1) / T{42}).Num() == 1);
    assert((R(1) / T{42}).Denom() == 42);
    assert(T{1} / (T{1} / (R)42) == (R)42);

    for (T n1 = -10; n1 <= 10; n1++) {
        for (T d1 = -10; d1 <= 10; d1++) {
            if (d1 == 0) {
                continue;
            }
            R ra = (R)n1 / d1;
            double da = (double)n1 / d1;
            T ta = n1 / d1;

            assert((double)ra == da);
            assert(RatNear(-ra, -da));
            assert((int)ra == (int)da);
            assert((float)ra == (float)da);
            assert((double)floor(ra) == floor(da));
            assert((double)ceil(ra) == ceil(da));
            assert((double)trunc(ra) == trunc(da));
            assert((double)round(ra) == round(da));
            assert((double)abs(ra) == abs(da));

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

                    assert(RatNear(ra + rb, da + db));
                    assert(RatNear(ra + ib, da + ib));
                    assert(RatNear(ta + rb, ta + db));

                    assert(RatNear(ra - rb, da - db));
                    assert(RatNear(ra - ib, da - ib));
                    assert(RatNear(ta - rb, ta - db));

                    assert(RatNear(ra * rb, da * db));
                    assert(RatNear(ra * ib, da * ib));
                    assert(RatNear(ta * rb, ta * db));

                    if (ib != 0) {
                        assert(RatNear(ra / rb, da / db));
                        assert(RatNear(ra / ib, da / ib));
                        assert(RatNear(ta / rb, ta / db));
                    }
                }
            }
        }
    }
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

    TestRational<int>("Rat");
    TestRational<long>("LRat");
    TestRational<long long>("LLRat");
    assert((LLRat)((Rat)2 / 3) == (LLRat)2LL / 3LL);
    assert((Rat)((LLRat)2LL / 3LL) == (Rat)2 / 3);
    
    std::cerr << "OK" << std::endl;
    return 0;
}