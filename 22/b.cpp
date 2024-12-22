#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <queue>
#include <ranges>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "collections.h"
#include "numbers.h"
#include "order.h"
#include "parse.h"

struct Hasher {
    size_t operator()(const NTuple<4, int>& x) const {
        const auto& [a, b, c, d] = x;
        return a + b * 40 + c * 1600 + d * 64000;
    }
};

const long long kMod = 16777216;

long long Step(long long x) {
    x = (x ^ (x * 64)) % kMod;
    x = (x ^ (x / 32)) % kMod;
    x = (x ^ (x * 2048)) % kMod;
    return x;
}

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), '\n');
    std::unordered_map<NTuple<4, int>, int, Hasher> counts;
    
    for (const auto& line : lines) {
        long long x = std::stoll(line);
        NTuple<4, int> t;
        std::unordered_set<NTuple<4, int>, Hasher> seen;

        for (int i = 0; i < 2000; i++) {
            long long next = Step(x);
            int change = (next % 10) - (x % 10);
            x = next;

            const auto& [_, b, c, d] = t;
            t = std::make_tuple(b, c, d, change);
            if (i < 3) {
                continue;
            }

            if (!seen.contains(t)) {
                seen.insert(t);
                counts[t] += x % 10;
            }
        }
    }

    std::cout << std::ranges::max(counts | std::views::values) << std::endl;
    return 0;
}