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
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), "\n");
    for (const std::string& line : lines) {
        std::vector<int> v = ParseVector<int>(line);
        assert(v.size() == 3);

        Vect cube = {v[0], v[1], v[2]};
        cubes.insert(cube);
    }

    int answer = 0;
    for (const Vect& u : cubes) {
        for (const Vect& dir : kDirs6) {
            if (!cubes.contains(u + dir)) {
                answer++;
            }
        }
    }

    std::cout << answer << std::endl;
    return 0;
}