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
    Coord pos = {0, 0};
    visited.insert(pos);
    for (char c : Trim(GetContents("input.txt"))) {
        pos += kDirArrows.at(c);
        visited.insert(pos);
    }
    std::cout << visited.size() << std::endl;
    return 0;
}