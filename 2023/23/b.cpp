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
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), "\n");
    Box box = Sizes<2>(input);
    Coord start = {0, 1};
    Coord finish = {box.size_i - 1, box.size_j - 2};

    // Naïve recursion takes over 2 minutes but works.
    int answer = 0;
    auto visit = [&](auto& self, Coord u, int depth) -> void {
        if (u == finish) {
            answer = std::max(answer, depth);
            return;
        }

        char c = std::exchange(input[u.i][u.j], '#');
        for (Coord v : Adj4(u)) {
            if (box.contains(v) && input[v.i][v.j] != '#') {
                self(self, v, depth + 1);
            }
        }
        input[u.i][u.j] = c;
    };
    visit(visit, start, 0);
    std::cout << answer << std::endl;
    return 0;
}