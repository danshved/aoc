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
    Coord start = FindOrDie<2>(input, 'S');

    std::unordered_map<Coord, int> d;
    DFSFrom(start, [&](auto& search, Coord u) {
        d[u] = search.Path().size() - 1;
        for (Coord v : Adj4(u)) {
            if (box.contains(v) && input[v.i][v.j] != '#') {
                search.Look(v);
            }
        }
    });

    int answer = 0;
    for (Coord x :box) {
        if (input[x.i][x.j] != '#') {
            continue;
        }
        for (Coord u : Adj4(x)) {
            if (!box.contains(u) || input[u.i][u.j] == '#') {
                continue;
            }
            for (Coord v : Adj4(x)) {
                if (u == v) {
                    continue;
                }
                if (!box.contains(v) || input[v.i][v.j] == '#') {
                    continue;
                }
                int cheat = d.at(v) - d.at(u) - 2;
                if (cheat >= 100) {
                    answer++;
                }
            }
        }
    }

    std::cout << answer << std::endl;
    return 0;
}