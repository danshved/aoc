#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
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

    int answer = 0;
    int area, perimeter;
    DFS<Coord>(
        [&](auto& search) {
            for (Coord u : Bounds(size_i, size_j)) {
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
            for (Coord dir : kDirs) {
                Coord v = u + dir;
                if (!InBounds(v, size_i, size_j) || input[v.i][v.j] != input[u.i][u.j]) {
                    perimeter++;
                } else {
                    search.Look(v);
                }
            }
        });

    std::cout << answer << std::endl;
    return 0;
}