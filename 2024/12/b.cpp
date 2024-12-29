#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#include "collections.h"
#include "graph_search.h"
#include "grid.h"
#include "parse.h"

int main() {
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), "\n");
    Box box = Sizes<2>(input);

    int answer = 0;
    int area;
    std::unordered_set<PosDir> border;
    DFS<Coord>(
        [&](auto& search) {
            for (Coord u : box) {
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
                    });

                answer += area * sides;
            }
        },
        [&](auto& search, Coord u) {
            if (search.Depth() == 0) {
                area = 0;
                border.clear();
            }

            area++;
            for (Coord v : Adj4(u)) {
                if (!box.contains(v) || input[v.i][v.j] != input[u.i][u.j]) {
                    border.insert({u, v - u});
                } else {
                    search.Look(v);
                }
            }
        });

    std::cout << answer << std::endl;
    return 0;
}