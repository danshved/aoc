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

int main() {
    std::unordered_set<Coord> visited;
    Coord santa = {0, 0}, robot = {0, 0};
    visited.insert(santa);
    for (char c : Trim(GetContents("input.txt"))) {
        santa += kDirArrows.at(c);
        visited.insert(santa);
        std::swap(santa, robot);
    }
    std::cout << visited.size() << std::endl;
    return 0;
}