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

const int kKeypad[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
const std::unordered_map<char, Coord> kCommands = {
    {'U', kNorth}, {'D', kSouth}, {'L', kWest}, {'R', kEast}};

int main() {
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), "\n");
    Coord pos = {1, 1};
    for (const std::string& line : input) {
        for (char c : line) {
            Coord v = pos + kCommands.at(c);
            if (Box(3, 3).contains(v)) {
                pos = v;
            }
        }
        std::cout << kKeypad[pos.i][pos.j];
    }
    std::cout << std::endl;
    return 0;
}