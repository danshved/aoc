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

    NestedVector<2, int> energy = ConstVector(0, box.size_i, box.size_j);
    for (Coord c : box) {
        energy[c.i][c.j] = input[c.i][c.j] - '0';
    }

    int answer = 0;
    for (int step = 0; step < 100; step++) {
        BFS<Coord>(
            [&](auto& search) {
                for (Coord u : box) {
                    if (++energy[u.i][u.j] >= 10) {
                        search.Look(u);
                    }
                }
            },
            [&](auto& search, Coord u) {
                answer++;
                for (Coord v : Adj8(u)) {
                    if (box.contains(v) && ++energy[v.i][v.j] >= 10) {
                        search.Look(v);
                    }
                }
            });
        for (Coord u : box) {
            if (energy[u.i][u.j] >= 10) {
                energy[u.i][u.j] = 0;
            }
        }
    };

    std::cout << answer << std::endl;
    return 0;
}