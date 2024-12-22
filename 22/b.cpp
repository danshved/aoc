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

using Tuple4 = std::tuple<int, int, int, int>;

struct Hasher {
    size_t operator()(const Tuple4& x) const {
        auto [a, b, c, d] = x;
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

    std::unordered_map<Tuple4, int, Hasher> counts;
    for (const auto& line : lines) {
        long long x = std::stoll(line);
        int changes[4] = {0, 0, 0, 0};
        int pos = 0;
        std::unordered_set<Tuple4, Hasher> seen;

        for (int i = 0; i < 2000; i++) {
            long long next = Step(x);
            int change = (next % 10) - (x % 10) + 18;
            x = next;

            changes[pos] = change;
            pos = (pos + 1) % 4;
            if (i < 3) {
                continue;
            }

            Tuple4 t = std::make_tuple(changes[pos], changes[(pos + 1) % 4],
                                       changes[(pos + 2) % 4], changes[(pos + 3) % 4]);
            if (!seen.contains(t)) {
                seen.insert(t);
                counts[t] += x % 10;
            }
        }
    }

    int answer = 0;
    for (const auto& [_, count] : counts) {
        answer = std::max(answer, count);
    }
    
    std::cout << answer << std::endl;
    return 0;
}