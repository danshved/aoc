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
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), "\n");
    Box box = Sizes<2>(input);
    Coord start = FindOrDie<2>(input, 'S');
    Coord end = FindOrDie<2>(input, 'E');

    BFSResult<Coord> d = BFS<Coord>(
        [&](auto& search) {
            for (Coord u : box) {
                if (GetHeight(input[u.i][u.j]) == GetHeight('a')) {
                    search.Look(u);
                }
            }
        },
        [&](auto& search, Coord u) {
            for (Coord v : Adj4(u)) {
                if (box.contains(v) && GetHeight(input[v.i][v.j]) <= GetHeight(input[u.i][u.j]) + 1) {
                    search.Look(v);
                }
            }
        });
    std::cout << d.at(end) << std::endl;
    return 0;
}