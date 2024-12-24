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
    int area;
    std::unordered_set<PosDir> border;
    DFS<Coord>(
        [&](auto& search) {
            for (Coord u : Bounds(size_i, size_j)) {
                if (search.Look(u) != DFSEdge::kTree) {
                    continue;
                }

                int sides = 0;
                DFS<PosDir>(
                    [&](auto& inner) {
                        for (const PosDir& x : border) {
                            if (inner.Look(x) == DFSEdge::kTree) {
                                sides++;
                            }
                        }
                    },
                    [&](auto& inner, const PosDir& x) {
                        if (border.contains(x.StrafeRight())) {
                            inner.Look(x.StrafeRight());
                        }
                        if (border.contains(x.StrafeLeft())) {
                            inner.Look(x.StrafeLeft());
                        }
                    }
                );

                answer += area * sides;
            }
        },
        [&](auto& search, Coord u) {
            if (search.Depth() == 0) {
                area = 0;
                border.clear();
            }
            
            area++;
            for (Coord dir : kDirs) {
                Coord v = u + dir;
                if (!InBounds(v, size_i, size_j) || input[v.i][v.j] != input[u.i][u.j]) {
                    border.insert({u, dir});
                } else {
                    search.Look(v);
                }
            }
        });

    std::cout << answer << std::endl;
    return 0;
}