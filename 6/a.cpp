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

    Coord dir = kNorth;
    Coord guard;
    std::tie(guard.i, guard.j) = FindOrDie<2>(input, '^');

    std::unordered_set<Coord> visited;
    while (true) {
        visited.insert(guard);
        Coord next = guard + dir;
        if (!InBounds(next, size_i, size_j)) {
            break;
        } else if (input[next.i][next.j] == '#') {
            dir = dir.RotateRight();
        } else {
            guard = next;
        }
    }

    std::cout << visited.size() << std::endl;
    return 0;
}