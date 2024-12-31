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

    bool operator==(const Vect&) const = default;
};

struct Brick {
    Vect from;
    Vect to;

    bool operator==(const Brick&) const = default;
};

template <>
struct std::hash<Brick> {
    size_t operator()(const Brick& b) const {
        return SeqHash(b.from.x, b.from.y, b.from.z, b.to.x, b.to.y, b.to.z);
    }
};

Brick ParseBrick(const std::string& s) {
    auto [sa, sb] = SplitN(s, "~");
    std::vector<int> va = ParseVector<int>(sa);
    std::vector<int> vb = ParseVector<int>(sb);
    for (int i = 0; i < 3; i++) {
        assert(va[i] <= vb[i]);
    }
    return {{va[0], va[1], va[2]}, {vb[0], vb[1], vb[2]}};
};

// Returns a  nonnegative value if brick `v` is above brick `u` and their
// XY-projections overlap. Returns nullopt otherwise.
std::optional<int> Weight(const Brick& u, const Brick& v) {
    if (v.from.x <= u.to.x && u.from.x <= v.to.x &&
        v.from.y <= u.to.y && u.from.y <= v.to.y && v.from.z > u.to.z) {
        return v.from.z - u.to.z - 1;
    } else {
        return std::nullopt;
    }
}

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), "\n");
    std::vector<Brick> bricks;
    for (const std::string& s : lines) {
        bricks.push_back(ParseBrick(s));
    }

    // Determine how far each brick will fall.
    std::unordered_map<Brick, int> fall = Dijkstra<Brick, int>(
        [&](auto& search) {
            for (const Brick& u : bricks) {
                search.Look(u, u.from.z - 1);
            }
        },
        [&](auto& search, const Brick& u, int d) {
            for (const Brick& v : bricks) {
                std::optional<int> w = Weight(u, v);
                if (w.has_value()) {
                    search.Look(v, d + *w);
                }
            }
        });

    // Determine which edges are "flex", i.e. which pairs of bricks lie
    // on each other after falling.
    std::unordered_map<Brick, std::vector<Brick>> parents;
    for (const Brick& u : bricks) {
        for (const Brick& v : bricks) {
            if (Weight(u, v) == fall.at(v) - fall.at(u)) {
                parents[v].push_back(u);
            }
        }
    }

    // Find which bricks are an only parent.
    std::unordered_set<Brick> only_parents;
    for (const Brick& u : bricks) {
        if (parents[u].size() == 1) {
            only_parents.insert(parents[u].front());
        }
    }

    std::cout << bricks.size() - only_parents.size() << std::endl;
    return 0;
}