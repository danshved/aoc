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

const std::unordered_map<std::string, Coord> kDirMap = {
    {"U", kNorth}, {"D", kSouth}, {"R", kEast}, {"L", kWest}};

int main() {
    Coord pos = {0, 0};
    int double_area = 0;
    int border = 0;

    for (const std::string& line : Split(Trim(GetContents("input.txt")), '\n')) {
        auto ss = Split(line, ' ');
        Coord dir = kDirMap.at(ss[0]);
        int steps = std::stoi(ss[1]);

        pos += dir * steps;
        double_area += (pos.i * dir.j - pos.j * dir.i) * steps;
        border += steps;
    }

    assert((pos == Coord{0, 0}));
    std::cout << ((abs(double_area) + border) / 2 + 1) << std::endl;
    return 0;
}