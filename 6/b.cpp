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
    auto [i, j] = FindOrDie<2>(input, '^');
    PosDir start = {{i, j}, kNorth};

    int count = 0;
    for (Coord obs : Bounds(size_i, size_j)) {
        std::cout << obs.i << " " << obs.j << std::endl;
        if (input[obs.i][obs.j] != '.') {
            continue;
        }

        std::unordered_set<PosDir> visited;
        PosDir guard = start;
        while (true) {
            if (auto [_, inserted] = visited.insert(guard); !inserted) {
                count++;
                break;
            }

            PosDir next = guard.Step();
            if (!InBounds(next.pos, size_i, size_j)) {
                break;
            } else if (input[next.pos.i][next.pos.j] == '#' || next.pos == obs) {
                guard = guard.RotateRight();
            } else {
                guard = next;
            }
        }
    }

    std::cout << count << std::endl;
    return 0;
}