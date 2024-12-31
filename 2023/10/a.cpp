#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
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

std::unordered_map<char, std::unordered_set<Coord>> pipes = {
    {'S', {kNorth, kWest, kSouth, kEast}},
    {'|', {kNorth, kSouth}},
    {'-', {kEast, kWest}},
    {'L', {kNorth, kEast}},
    {'F', {kSouth, kEast}},
    {'J', {kNorth, kWest}},
    {'7', {kSouth, kWest}},
};

int main() {
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), "\n");
    Box box = Sizes<2>(input);
    Coord start = FindOrDie<2>(input, 'S');

    int count = 0;
    BFSFrom(start, [&](auto& search, Coord u) {
        count++;
        for (Coord dir : pipes[input[u.i][u.j]]) {
            Coord v = u + dir;
            if (!box.contains(v)) {
                continue;
            }
            if (!pipes[input[v.i][v.j]].contains(dir.Flip())) {
                continue;
            }
            search.Look(v);
        }
    });

    std::cout << (count / 2) << std::endl;
    return 0;
}