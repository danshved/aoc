#include <iostream>
#include <ranges>
#include <string>
#include <unordered_set>
#include <vector>

#include "collections.h"
#include "grid.h"
#include "parse.h"

const Box kBox = {71, 71};

int main() {
    std::vector<Coord> bytes;
    for (const std::string& line : Split(Trim(GetContents("input.txt")), "\n")) {
        auto [l, r] = SplitN(line, ",");
        bytes.push_back(Coord{std::stoi(l), std::stoi(r)});
    }

    // Let all the bytes fall down.
    std::unordered_set<Coord> occupied(bytes.begin(), bytes.end());
    assert(occupied.size() == bytes.size());

    // See what's reachable with all the bytes on the ground.
    std::unordered_set<Coord> reachable;
    auto expand = [&](auto& self, const Coord& u) -> bool {
        reachable.insert(u);
        for (Coord v : Adj4(u)) {
            if (kBox.contains(v) && !occupied.contains(v) && !reachable.contains(v)) {
                self(self, v);
            }
        }
        return reachable.contains({kBox.size_i - 1, kBox.size_j - 1});
    };
    assert(!expand(expand, {0, 0}));

    // Lift the bytes one by one until the corner is reachable.
    for (const Coord& b : bytes | std::views::reverse) {
        occupied.erase(b);
        if (std::ranges::none_of(Adj4(b), [&](const Coord& n) {
                return reachable.contains(n);
            })) {
            continue;
        }
        if (expand(expand, b)) {
            std::cout << b.i << "," << b.j << std::endl;
            break;
        }
    }
    return 0;
}