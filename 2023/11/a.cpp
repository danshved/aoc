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
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), '\n');
    auto [size_i, size_j] = Sizes<2>(input);

    std::vector<Coord> galaxies;
    std::vector<int> expand_i(size_i, 2), expand_j(size_j, 2), real_i, real_j;
    for (auto [i, j] : Bounds(size_i, size_j)) {
        if (input[i][j] == '#') {
            expand_i[i] = 1;
            expand_j[j] = 1;
            galaxies.push_back({i, j});
        }
    }
    std::partial_sum(expand_i.begin(), expand_i.end(), std::back_inserter(real_i));
    std::partial_sum(expand_j.begin(), expand_j.end(), std::back_inserter(real_j));
    auto Expand = [&](Coord c) -> Coord {
        return {real_i[c.i], real_j[c.j]};
    };

    int answer = 0;
    for (Coord a : galaxies) {
        for (Coord b : galaxies) {
            if (a >= b) {
                continue;
            }
            answer += (Expand(a) - Expand(b)).Manhattan();
        }
    }
    std::cout << answer << std::endl;
    return 0;
}