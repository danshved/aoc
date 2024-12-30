#include <iostream>
#include <string>
#include <vector>

#include "collections.h"
#include "graph_search.h"
#include "grid.h"
#include "parse.h"

int main() {
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), "\n");
    Box box = Sizes<2>(input);

    std::vector<int> basins;
    int basin_size = 0;
    DFS<Coord>(
        [&](auto& search) {
            for (Coord u : box) {
                if (input[u.i][u.j] != '9' && search.Look(u) == DFSEdge::kTree) {
                    basins.push_back(basin_size);
                }
            }
        },
        [&](auto& search, Coord u) {
            if (search.Depth() == 0) {
                basin_size = 0;
            }
            basin_size++;
            for (Coord v : Adj4(u)) {
                if (box.contains(v) && input[v.i][v.j] != '9') {
                    search.Look(v);
                }
            }
        });

    assert(basins.size() >= 3);
    std::sort(basins.begin(), basins.end(), std::greater<int>());
    std::cout << basins[0] * basins[1] * basins[2] << std::endl;
    return 0;
}