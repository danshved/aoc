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
#include "graph_search.h"
#include "grid.h"
#include "numbers.h"
#include "order.h"
#include "parse.h"

int main() {
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), "\n");
    Box box = Sizes<2>(input);

    auto count = [&](const PosDir& start) {
        std::unordered_set<Coord> energized;
        BFSFrom(start, [&](auto& search, PosDir u) {
            u = u.Step();
            if (!box.contains(u.pos)) {
                return;
            }
            energized.insert(u.pos);

            switch (input[u.pos.i][u.pos.j]) {
                case '.':
                    search.Look(u);
                    break;
                case '-':
                    if (u.dir.i == 0) {
                        search.Look(u);
                    } else {
                        search.Look(u.RotateRight());
                        search.Look(u.RotateLeft());
                    }
                    break;
                case '|':
                    if (u.dir.j == 0) {
                        search.Look(u);
                    } else {
                        search.Look(u.RotateRight());
                        search.Look(u.RotateLeft());
                    }
                    break;
                case '/':
                    search.Look((u.dir.i == 0) ? u.RotateLeft() : u.RotateRight());
                    break;
                case '\\':
                    search.Look((u.dir.j == 0) ? u.RotateLeft() : u.RotateRight());
                    break;
                default:
                    assert(false);
            }
        });
        return (int)energized.size();
    };

    int answer = 0;
    for (int i = 0; i < box.size_i; i++) {
        answer = std::max(answer, count({{i, -1}, kEast}));
        answer = std::max(answer, count({{i, box.size_j}, kWest}));
    }
    for (int j = 0; j < box.size_j; j++) {
        answer = std::max(answer, count({{-1, j}, kSouth}));
        answer = std::max(answer, count({{box.size_i, j}, kNorth}));
    }
    std::cout << answer << std::endl;
    return 0;
}