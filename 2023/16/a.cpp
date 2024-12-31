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

    std::unordered_set<Coord> energized;
    BFSFrom(PosDir{{0, -1}, kEast}, [&](auto& search, PosDir u) {
        u = u.Step();
        if (!box.contains(u.pos)) {
            return;
        }
        energized.insert(u.pos);

        switch(input[u.pos.i][u.pos.j]) {
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

    std::cout << energized.size() << std::endl;
    return 0;
}