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
    for (Coord u : box) {
        if (input[u.i][u.j] == '#') {
            continue;
        }
        for (Coord v : ManhattanSpiral(u)) {
            int md = (u - v).Manhattan();
            if (md > 20) {
                break;
            }
            if (!box.contains(v) || input[v.i][v.j] == '#') {
                continue;
            }

            int cheat = d.at(v) - d.at(u) - md;
            if (cheat >= 100) {
                answer++;
            }
        }
    }

    std::cout << answer << std::endl;
    return 0;
}