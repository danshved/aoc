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
    PosDir start = {FindOrDie<2>(input, 'S'), kEast};
    PosDir end = {FindOrDie<2>(input, 'E'), kNorth};

    std::unordered_map<PosDir, int> dist =
        DijkstraFrom(start, 0, [&](auto& search, const PosDir& u, int d) {
            search.Look(u.RotateRight(), d + 1000);
            search.Look(u.RotateLeft(), d + 1000);
            PosDir v = u.Step();
            if (box.contains(v.pos) && input[v.pos.i][v.pos.j] != '#') {
                search.Look(v, d + 1);
            }
        });

    std::cout << dist.at(end) << std::endl;
    return 0;
}