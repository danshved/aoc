#include <iostream>
#include <limits>
#include <ranges>
#include <string>
#include <vector>

#include "collections.h"
#include "graph_search.h"
#include "grid.h"
#include "parse.h"

const Box kBox = {71, 71};

int main() {
    std::vector<Coord> bytes;
    for (const std::string& line : Split(Trim(GetContents("input.txt")), "\n")) {
        auto [l, r] = SplitN(line, ",");
        bytes.push_back(Coord{std::stoi(l), std::stoi(r)});
    }

    NestedVector<2, bool> occupied = ConstVector(false, kBox.size_i, kBox.size_j);
    for (const Coord& b : bytes) {
        occupied[b.i][b.j] = true;
        BFSResult<Coord> d = BFSFrom(Coord{0, 0}, [&](auto& search, const Coord& u) {
            for (Coord v : Adj4(u)) {
                if (kBox.contains(v) && !occupied[v.i][v.j]) {
                    search.Look(v);
                }
            }
        });
        if (!d.contains({kBox.size_i - 1, kBox.size_j - 1})) {
            std::cout << b.i << "," << b.j << std::endl;
            break;
        }
    }
    return 0;
}