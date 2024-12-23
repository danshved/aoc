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

int main() {
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), '\n');
    auto [size_i, size_j] = Sizes<2>(input);
    Coord start = FindOrDie<2>(input, 'S');

    std::unordered_map<Coord, int> d;
    DFSFrom(start, [&](auto& search, const Coord& u) {
        d[u] = search.Path().size() - 1;
        for (Coord dir : kDirs) {
            Coord v = u + dir;
            if (InBounds(v, size_i, size_j) && input[v.i][v.j] != '#') {
                search.Look(v);
            }
        }
    });

    int answer = 0;
    for (Coord u : Bounds(size_i, size_j)) {
        if (input[u.i][u.j] == '#') {
            continue;
        }
        for (Coord v : Bounds(size_i, size_j)) {
            if (input[v.i][v.j] == '#') {
                continue;
            }
            int md = (u - v).Manhattan();
            if (md > 20) {
                continue;
            }
            int cheat = d.at(v) - d.at(u) - md;
            if (cheat >= 100) {
                answer++;
            }
        }
    }

    std::cout << answer << std::endl;
    return 0;
}