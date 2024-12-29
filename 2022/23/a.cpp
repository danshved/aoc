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

const Coord kMoves[4] = {kNorth, kSouth, kWest, kEast};

int main() {
    std::vector<std::string> matrix = Split(Trim(GetContents("input.txt")), '\n');
    auto [size_i, size_j] = Sizes<2>(matrix);

    std::unordered_set<Coord> elves;
    for (Coord c : Bounds(size_i, size_j)) {
        if (matrix[c.i][c.j] == '#') {
            elves.insert(c);
        }
    }

    int step;
    for (step = 0; step < 10; step++) {
        auto occupied = [&](const Coord& c) { return elves.contains(c); };
        std::unordered_map<Coord, std::vector<Coord>> candidates;
        for (Coord elf : elves) {
            if (std::ranges::none_of(Adj8(elf), occupied)) {
                continue;
            }
            for (int t = 0; t < 4; t++) {
                Coord dir = kMoves[(step + t) % 4];
                Coord check[3] = {elf + dir, elf + dir + dir.RotateLeft(),
                                  elf + dir + dir.RotateRight()};
                if (std::ranges::none_of(check, occupied)) {
                    candidates[elf + dir].push_back(elf);
                    break;
                }
            }
        }

        for (const auto& [to, from] : candidates) {
            if (from.size() == 1) {
                elves.erase(from[0]);
                elves.insert(to);
            }
        }
    }

    auto get_i = [](const Coord& c) { return c.i; };
    auto get_j = [](const Coord& c) { return c.j; };
    int min_i = std::ranges::min(elves | std::views::transform(get_i));
    int max_i = std::ranges::max(elves | std::views::transform(get_i));
    int min_j = std::ranges::min(elves | std::views::transform(get_j));
    int max_j = std::ranges::max(elves | std::views::transform(get_j));
    int answer = (max_i - min_i + 1) * (max_j - min_j + 1) - elves.size();
    std::cout << answer << std::endl;
    return 0;
}