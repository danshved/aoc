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
    std::unordered_map<Coord, int> d =
        DijkstraFrom(Coord{0, 0}, 0, [&](auto& search, Coord u, int d) {
            for (Coord v : Adj4(u)) {
                if (box.contains(v)) {
                    search.Look(v, d + input[v.i][v.j] - '0');
                }
            }
        });
    std::cout << d[Coord{box.size_i - 1, box.size_j - 1}] << std::endl;
    return 0;
}