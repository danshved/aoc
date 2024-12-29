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

    int answer = 0;
    BFSFrom(start, [&](auto& search, Coord u) {
        if (search.Depth() <= 64 && search.Depth() % 2 == 0) {
            answer++;
        }
        for (Coord dir : kDirs) {
            Coord v = u + dir;
            if (InBounds(v, size_i, size_j) && input[v.i][v.j] != '#') {
                search.Look(v);
            }
        }
    });

    std::cout << answer << std::endl;
    return 0;
}