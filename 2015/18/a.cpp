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
    std::vector<std::string> matrix = Split(Trim(GetContents("input.txt")), "\n");
    Box box = Sizes<2>(matrix);

    for (int step = 0; step < 100; step++) {
        std::vector<std::string> next(box.size_i, std::string(box.size_j, '.'));
        for (Coord u : box) {
            int neighbors = std::ranges::count_if(Adj8(u), [&](const Coord& v){
                return box.contains(v) && matrix[v.i][v.j] == '#';
            });
            if (matrix[u.i][u.j] == '#') {
                next[u.i][u.j] = (neighbors == 2 || neighbors == 3) ? '#' : '.';
            } else {
                next[u.i][u.j] = (neighbors == 3) ? '#' : '.';
            }
        }
        matrix = std::move(next);
    }

    int answer = std::ranges::count_if(box, [&](const Coord &u) {
        return matrix[u.i][u.j] == '#';
    });
    std::cout << answer << std::endl;
    return 0;
}