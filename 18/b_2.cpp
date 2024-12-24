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
#include "graph_search.h"
#include "grid.h"
#include "numbers.h"
#include "order.h"
#include "parse.h"

const int kSizeI = 71;
const int kSizeJ = 71;

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), '\n');
    std::vector<Coord> bytes;
    for (const std::string& line : lines) {
        auto [l, r] = Split2(line, ',');
        bytes.push_back(Coord{std::stoi(l), std::stoi(r)});
    }

    // Let all the bytes fall down.
    NestedVector<2, int> byte_count = ConstVector(0, kSizeI, kSizeJ);
    for (const Coord& b : bytes) {
        byte_count[b.i][b.j]++;
    }

    NestedVector<2, bool> reachable = ConstVector(false, kSizeI, kSizeJ);
    auto expand = [&](const Coord& start) {
        DFSFrom(start, [&](auto& search, const Coord& u) {
            reachable[u.i][u.j] = true;
            for (Coord dir : kDirs) {
                Coord v = u + dir;
                if (InBounds(v, kSizeI, kSizeJ) && byte_count[v.i][v.j] == 0 &&
                    !reachable[v.i][v.j]) {
                    search.Look(v);
                }
            }
        });
        return reachable[kSizeI - 1][kSizeJ - 1];
    };

    // Lift the bytes one by one until the corner is reachable
    assert(!expand({0, 0}));
    for (const Coord& b : bytes | std::views::reverse) {
        byte_count[b.i][b.j]--;
        if (byte_count[b.i][b.j] > 0) {
            continue;
        }
        if (std::ranges::none_of(kDirs, [&](const Coord& dir) {
                Coord n = b + dir;
                return InBounds(n, kSizeI, kSizeJ) && reachable[n.i][n.j];
            })) {
            continue;
        }
        if (expand(b)) {
            std::cout << b.i << "," << b.j << std::endl;
            break;
        }
    }

    return 0;
}