#include <iostream>
#include <string>
#include <unordered_set>

#include "collections.h"
#include "parse.h"

const long long kMod = 16777216;

long long Step(long long x) {
    x = (x ^ (x * 64)) % kMod;
    x = (x ^ (x / 32)) % kMod;
    x = (x ^ (x * 2048)) % kMod;
    return x;
}

int main() {
    std::unordered_map<NTuple<4, int>, int, TupleHasher> counts;
    for (const auto& line : Split(Trim(GetContents("input.txt")), "\n")) {
        long long x = std::stoll(line);
        NTuple<4, int> t;
        std::unordered_set<NTuple<4, int>, TupleHasher> seen;

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