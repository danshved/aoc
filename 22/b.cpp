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

const long long kMod = 16777216;

long long Step(long long x) {
    x = (x ^ (x * 64)) % kMod;
    x = (x ^ (x / 32)) % kMod;
    x = (x ^ (x * 2048)) % kMod;
    return x;
}

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), '\n');

    NestedVector<4, int> counts = ConstVector(0, 37, 37, 37, 37);
    for (const auto& line : lines) {
        long long x = std::stoll(line);
        NestedVector<4, int> local_counts = ConstVector(-1, 37, 37, 37, 37);
        int changes[4] = {0, 0, 0, 0};
        int pos = 0;

        for (int i = 0; i < 2000; i++) {
            long long next = Step(x);
            int change = (next % 10) - (x % 10) + 18;
            x = next;

            changes[pos] = change;
            pos = (pos + 1) % 4;
            if (i < 3) {
                continue;
            }

            int& count = local_counts[changes[pos]][changes[(pos + 1) % 4]]
                                     [changes[(pos + 2) % 4]][changes[(pos + 3) % 4]];
            if (count == -1) {
                count = x % 10;
            }
        }

        for (int a = 0; a < 37; a++) {
            for (int b = 0; b < 37; b++) {
                for (int c = 0; c < 37; c++) {
                    for (int d = 0; d < 37; d++) {
                        if (local_counts[a][b][c][d] != -1) {
                            counts[a][b][c][d] += local_counts[a][b][c][d];
                        }
                    }
                }
            }
        }
    }

    int answer = 0;
    for (int a = 0; a < 37; a++) {
        for (int b = 0; b < 37; b++) {
            for (int c = 0; c < 37; c++) {
                for (int d = 0; d < 37; d++) {
                    answer = std::max(answer, counts[a][b][c][d]);
                }
            }
        }
    }

    std::cout << answer << std::endl;
    return 0;
}