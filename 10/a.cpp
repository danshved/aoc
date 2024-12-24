#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
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
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), '\n');
    auto [size_i, size_j] = Sizes<2>(input);

    int answer = 0;
    for (Coord start : Bounds(size_i, size_j)) {
        if (input[start.i][start.j] != '0') {
            continue;
        }
        DFSFrom(start, [&](auto& search, Coord u) {
            if (input[u.i][u.j] == '9') {
                answer++;
            }
            for (Coord dir : kDirs) {
                Coord v = u + dir;
                if (InBounds(v, size_i, size_j) && input[v.i][v.j] == input[u.i][u.j] + 1) {
                    search.Look(v);
                }
            }
        });
    }

    std::cout << answer << std::endl;
    return 0;
}