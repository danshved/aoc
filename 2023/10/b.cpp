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

    int border = 0;
    int double_area = 0;

    // This needs larger than default stack size with MinGW gcc, e.g.:
    // g++ -std=c++20 -O3 -ftrapv '-Wl,--stack,256000000' -o b.exe b.cpp
    DFSFrom<Coord>(start, [&](auto& search, const Coord& u) {
        border++;
        for (Coord dir : pipes.at(input[u.i][u.j])) {
            Coord v = u + dir;
            if (!box.contains(v)) {
                continue;
            }
            if (!pipes.at(input[v.i][v.j]).contains(dir.Flip())) {
                continue;
            }
            if (search.Parent() == v) {
                continue;
            }

            DFSEdge kind = search.Look(v);
            if (kind == DFSEdge::kTree || kind == DFSEdge::kBack) {
                double_area += u.i * v.j - u.j * v.i;
            }
        }
    });
    std::cout << (abs(double_area) / 2 - border / 2 + 1) << std::endl;
    return 0;
}