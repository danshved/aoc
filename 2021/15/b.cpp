#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "collections.h"
#include "graph_search.h"
#include "grid.h"
#include "parse.h"

int main() {
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), "\n");
    Box box = Sizes<2>(input);
    Box large_box = {5 * box.size_i, 5 * box.size_j};

    auto value = [&](Coord c) {
        Coord d = box.Wrap(c);
        int i = c.i / box.size_i, j = c.j / box.size_j;
        return 1 + (input[d.i][d.j] - '1' + i + j) % 9;
    };

    std::unordered_map<Coord, int> d =
        DijkstraFrom(Coord{0, 0}, 0, [&](auto& search, Coord u, int d) {
            for (Coord v : Adj4(u)) {
                if (large_box.contains(v)) {
                    search.Look(v, d + value(v));
                }
            }
        });
    std::cout << d[Coord{large_box.size_i - 1, large_box.size_j - 1}] << std::endl;
    return 0;
}