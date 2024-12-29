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

const std::unordered_map<std::string, Coord> kCmdToDir = {
    {"U", kNorth}, {"D", kSouth}, {"R", kEast}, {"L", kWest}
};

int Clamp(int i) {
    return std::max(-1, std::min(1, i));
}

Coord StepTowards(Coord c) {
    return {Clamp(c.i), Clamp(c.j)};
};

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), '\n');
    std::vector<Coord> rope(10, Coord{0, 0});
    std::unordered_set<Coord> visited;

    visited.insert(rope.back());
    for (const std::string& s : lines) {
        auto [cmd, count_s] = Split2(s, ' ');
        Coord dir = kCmdToDir.at(cmd);
        int count = std::stoi(count_s);
        for (int i = 0; i < count; i++) {
            rope[0] += dir;
            for (int i = 1; i < rope.size(); i++) {
                Coord diff = rope[i - 1] - rope[i];
                if (diff.Chess() > 1) {
                    rope[i] += StepTowards(diff);
                } else {
                    break;
                }
            }
            visited.insert(rope.back());
        }
    }

    std::cout << visited.size() << std::endl;
    return 0;
}