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

    int answer = 0;
    int area, perimeter;
    DFS<Coord>(
        [&](auto& search) {
            for (Coord u : box) {
                if (search.Look(u) == DFSEdge::kTree) {
                    answer += area * perimeter;
                }
            }
        },
        [&](auto& search, Coord u) {
            if (search.Depth() == 0) {
                area = 0;
                perimeter = 0;
            }
            area++;
            for (Coord v : Adj4(u)) {
                if (!box.contains(v) || input[v.i][v.j] != input[u.i][u.j]) {
                    perimeter++;
                } else {
                    search.Look(v);
                }
            }
        });

    std::cout << answer << std::endl;
    return 0;
}