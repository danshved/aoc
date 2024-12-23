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

struct Coord {
    int i, j;

    Coord operator+(const Coord& other) const {
        return {i + other.i, j + other.j};
    }

    bool operator==(const Coord&) const = default;
};

template<> struct std::hash<Coord> {
    size_t operator()(const Coord &c) const {
        return SeqHash(c.i, c.j);
    }
};

const Coord kDirs[4] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
const int kUp = 0;

int main() {
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), '\n');
    auto [size_i, size_j] = Sizes<2>(input);
    auto in_bounds = [&](Coord c) {
        return c.i >= 0 && c.i < size_i && c.j >= 0 && c.j < size_j;
    };

    int dir = kUp;
    Coord guard;
    std::tie(guard.i, guard.j) = FindOrDie<2>(input, '^');

    std::unordered_set<Coord> visited;
    while (in_bounds(guard)) {
        visited.insert(guard);
        Coord next = guard + kDirs[dir];
        if (in_bounds(next) && input[next.i][next.j] == '#') {
            dir = (dir + 1) % 4;
        } else {
            guard = next;
        }
    }

    std::cout << visited.size() << std::endl;
    return 0;
}