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

template <>
struct std::hash<Coord> {
    size_t operator()(const Coord& c) const {
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

    Coord start;
    std::tie(start.i, start.j) = FindOrDie<2>(input, '^');

    int count = 0;
    Coord obs;
    for (obs.i = 0; obs.i < size_i; ++obs.i) {
        for (obs.j = 0; obs.j < size_j; ++obs.j) {
            std::cout << obs.i << " " << obs.j << std::endl;

            if (input[obs.i][obs.j] != '.') {
                continue;
            }

            NestedVector<3, bool> visited = ConstVector(false, size_i, size_j, 4);
            Coord guard = start;
            int dir = kUp;
            bool loop = false;

            while (true) {
                if (visited[guard.i][guard.j][dir]) {
                    loop = true;
                    break;
                }
                visited[guard.i][guard.j][dir] = true;

                Coord next = guard + kDirs[dir];
                if (!in_bounds(next)) {
                    break;
                }
                if (input[next.i][next.j] == '#' || next == obs) {
                    dir = (dir + 1) % 4;
                } else {
                    guard = next;
                }
            }

            if (loop) {
                count++;
            }
        }
    }

    std::cout << count << std::endl;
    return 0;
}