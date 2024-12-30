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

    int step = 0;
    for (;; step++) {
        BFS<Coord>(
            [&](auto& search) {
                for (Coord u : box) {
                    if (++energy[u.i][u.j] >= 10) {
                        search.Look(u);
                    }
                }
            },
            [&](auto& search, Coord u) {
                for (Coord v : Adj8(u)) {
                    if (box.contains(v) && ++energy[v.i][v.j] >= 10) {
                        search.Look(v);
                    }
                }
            });
        int flashes = 0;
        for (Coord u : box) {
            if (energy[u.i][u.j] >= 10) {
                energy[u.i][u.j] = 0;
                flashes++;
            }
        }
        if (flashes == box.size_i * box.size_j) {
            break;
        }
    };

    std::cout << step + 1 << std::endl;
    return 0;
}