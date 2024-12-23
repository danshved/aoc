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
#include "grid.h"
#include "numbers.h"
#include "order.h"
#include "parse.h"

int main() {
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), '\n');
    auto [size_i, size_j] = Sizes<2>(input);
    Coord start;
    std::tie(start.i, start.j) = FindOrDie<2>(input, '^');

    int count = 0;
    for (Coord obs : Bounds(size_i, size_j)) {
        std::cout << obs.i << " " << obs.j << std::endl;
        if (input[obs.i][obs.j] != '.') {
            continue;
        }

        std::unordered_set<PosJump> visited;
        Coord guard = start;
        Coord dir = kNorth;
        while (true) {
            if (auto [_, ok] = visited.insert({guard, dir}); !ok) {
                count++;
                break;
            }

            Coord next = guard + dir;
            if (!InBounds(next, size_i, size_j)) {
                break;
            }
            if (input[next.i][next.j] == '#' || next == obs) {
                dir = dir.CW();
            } else {
                guard = next;
            }
        }
    }

    std::cout << count << std::endl;
    return 0;
}