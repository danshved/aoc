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

int GetHeight(char c) {
    return (c >= 'a' && c <= 'z') ? (c - 'a')
           : (c == 'S')           ? GetHeight('a')
           : (c == 'E')           ? GetHeight('z')
                                  : -1;
}

int main() {
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), '\n');
    auto [size_i, size_j] = Sizes<2>(input);
    Coord start = FindOrDie<2>(input, 'S');
    Coord end = FindOrDie<2>(input, 'E');

    BFSResult<Coord> d = BFSFrom(start, [&](auto& search, Coord u) {
        for (Coord dir : kDirs) {
            Coord v = u + dir;
            if (InBounds(v, size_i, size_j) &&
                GetHeight(input[v.i][v.j]) <= GetHeight(input[u.i][u.j]) + 1) {
                search.Look(v);
            }
        }
    });
    std::cout << d.at(end) << std::endl;
    return 0;
}