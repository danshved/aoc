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
#include "graph_search.h"
#include "grid.h"

const int kInf = std::numeric_limits<int>::max();

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
    for (int i = 0; i < 1024; i++) {
        occupied[bytes[i].i][bytes[i].j] = true;
    }

    BFSResult<Coord> d = BFSFrom(Coord{0, 0}, [&](auto& search, const Coord& u) {
        for (Coord dir : kDirs) {
            Coord v = u + dir;
            if (InBounds(v, kSizeI, kSizeJ) && !occupied[v.i][v.j]) {
                search.Look(v);
            }
        }
    });

    std::cout << d.at({kSizeI - 1, kSizeJ - 1}) << std::endl;
    return 0;
}