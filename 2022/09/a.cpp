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
    std::vector<std::string> lines = Split(Trim(GetContents("input2.txt")), "\n");
    Coord head, tail;
    std::unordered_set<Coord> visited;

    visited.insert(tail);
    for (const std::string& s : lines) {
        auto [cmd, count_s] = SplitN(s, " ");
        Coord dir = kCmdToDir.at(cmd);
        int count = std::stoi(count_s);
        for (int i = 0; i < count; i++) {
            head += dir;
            if ((head - tail).Chess() > 1) {
                tail += StepTowards(head - tail);
                visited.insert(tail);
            }
        }
    }

    std::cout << visited.size() << std::endl;
    return 0;
}