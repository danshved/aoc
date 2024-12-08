#include <cassert>
#include <iostream>
#include <tuple>

#include "numbers.h"
#include "rational.h"
#include "order.h"

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

    std::cerr << "Testing GetPrimes()..." << std::endl;
    assert((GetPrimes(20) == std::vector<int>{2, 3, 5, 7, 11, 13, 17, 19}));

    std::cerr << "OK" << std::endl;

    std::cout << (float)((Rat)1/2/((Rat)1 / 4 - (Rat)1 / 3)) << std::endl;

    return 0;
}