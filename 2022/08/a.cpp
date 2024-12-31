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

    std::unordered_map<PosDir, int> d;
    auto max_height = [&](auto &self, const PosDir& pd) {
        if (d.contains(pd)) {
            return d[pd];
        }

        PosDir next = pd.Step();
        if (!box.contains(next.pos)) {
            return d[pd] = -1;
        }

        return d[pd] = std::max(self(self, next), input[next.pos.i][next.pos.j] - '0');
    };
    auto is_visible = [&](Coord pos) {
        for (Coord dir : Adj4({0, 0})) {
            if (max_height(max_height, {pos, dir}) < input[pos.i][pos.j] - '0') {
                return true;
            }
        }
        return false;
    };

    std::cout << std::ranges::count_if(box, is_visible) << std::endl;    
    return 0;
}