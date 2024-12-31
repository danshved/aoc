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

struct Vect {
    int x, y, z;

    Vect operator+(const Vect& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }

    bool operator==(const Vect&) const = default;
};

template<>
struct std::hash<Vect>{
    size_t operator()(const Vect& v) const {
        return SeqHash(v.x, v.y, v.z);
    }
};

const Vect kDirs6[6] = {{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}};

int main() {
    std::unordered_set<Vect> cubes;
    int min_x = 0, min_y = 0, min_z = 0, max_x = 0, max_y = 0, max_z = 0;

    for (const std::string& line : Split(Trim(GetContents("input.txt")), "\n")) {
        std::vector<int> v = ParseVector<int>(line);
        assert(v.size() == 3);

        Vect cube = {v[0], v[1], v[2]};
        cubes.insert(cube);
        min_x = std::min(min_x, cube.x - 1);
        min_y = std::min(min_y, cube.y - 1);
        min_z = std::min(min_z, cube.z - 1);
        max_x = std::max(max_x, cube.x + 1);
        max_y = std::max(max_y, cube.y + 1);
        max_z = std::max(max_z, cube.z + 1);
    }

    int answer = 0;
    BFSFrom(Vect{min_x, min_y, min_z}, [&](auto& search, const Vect& u) {
        for (const Vect& dir : kDirs6) {
            Vect v = u + dir;
            if (v.x < min_x || v.y < min_y || v.z < min_z ||
                v.x > max_x || v.y > max_y || v.z > max_z) {
                    continue;
                }
            if (cubes.contains(v)) {
                answer++;
            } else {
                search.Look(v);
            }
        }
    });

    std::cout << answer << std::endl;
    return 0;
}