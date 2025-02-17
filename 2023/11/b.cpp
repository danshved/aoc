#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <optional>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "collections.h"
#include "grid.h"
#include "numbers.h"
#include "order.h"
#include "parse.h"

int main() {
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), "\n");
    Box box = Sizes<2>(input);

    std::vector<Coord> galaxies;
    std::vector<int> expand_i(box.size_i, 1000000), expand_j(box.size_j, 1000000), real_i, real_j;
    for (auto [i, j] : box) {
        if (input[i][j] == '#') {
            expand_i[i] = 1;
            expand_j[j] = 1;
            galaxies.push_back({i, j});
        }
    }
    std::partial_sum(expand_i.begin(), expand_i.end(), std::back_inserter(real_i));
    std::partial_sum(expand_j.begin(), expand_j.end(), std::back_inserter(real_j));
    auto expand = [&](Coord c) -> Coord {
        return {real_i[c.i], real_j[c.j]};
    };

    long long answer = 0;
    for (Coord a : galaxies) {
        for (Coord b : galaxies) {
            if (a >= b) {
                continue;
            }
            answer += (expand(a) - expand(b)).Manhattan();
        }
    }
    std::cout << answer << std::endl;
    return 0;
}