#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <optional>
#include <ranges>
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
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), "\n");
    Box box = Sizes<2>(input);
    Coord start = FindOrDie<2>(input, 'S');

    std::unordered_map<Coord, long long> counts;
    DFSFrom(start, [&](auto& search, Coord u) {
        Coord below{u.i + 1, u.j}, left{u.i + 1, u.j - 1}, right{u.i + 1, u.j + 1};
        if (!box.contains(below)) {
            counts[u] = 1;
        } else if (input[below.i][below.j] == '.') {
            search.Look(below);
            counts[u] = counts[below];
        } else {
            search.Look(left);
            search.Look(right);
            counts[u] = counts[left] + counts[right];
        }
    });

    std::cout << counts[start] << std::endl;
    return 0;
}