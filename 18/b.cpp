#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <queue>
#include <ranges>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "collections.h"
#include "numbers.h"
#include "order.h"
#include "parse.h"
#include "grid.h"
#include "graph_search.h"

const int kSizeI = 71;
const int kSizeJ = 71;

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), '\n');
    std::vector<Coord> bytes;
    for (const std::string& line : lines) {
        auto [l, r] = Split2(line, ',');
        bytes.push_back(Coord{std::stoi(l), std::stoi(r)});
    }

    NestedVector<2, bool> occupied = ConstVector(false, kSizeI, kSizeJ);
    int i;
    for (i = 0; i < bytes.size(); i++) {
        occupied[bytes[i].i][bytes[i].j] = true;

        BFSResult<Coord> d = BFSFrom(Coord{0, 0}, [&](auto& search, const Coord& u) {
            for (Coord dir : kDirs) {
                Coord v = u + dir;
                if (InBounds(v, kSizeI, kSizeJ) && !occupied[v.i][v.j]) {
                    search.Look(v);
                }
            }
        });

        if (!d.contains({kSizeI - 1, kSizeJ - 1})) {
            break;
        }
    }

    std::cout << bytes[i].i << "," << bytes[i].j << std::endl;
    return 0;
}