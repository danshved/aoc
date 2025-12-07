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
    std::cout << start.i << " " << start.j << std::endl;

    std::unordered_map<int, long long> beams = {{start.j, 1}};
    for (int i = 0; i < box.size_i - 1; i++) {
        std::cout << "row " << i << std::endl;
        std::unordered_map<int, long long> new_beams;
        for (auto [beam, count] : beams) {
            if (input[i + 1][beam] == '.') {
                new_beams[beam] += count;
            } else {
                if (beam - 1 >= 0) {
                    new_beams[beam - 1] += count;
                }
                if (beam + 1 < box.size_j) {
                    new_beams[beam + 1] += count;
                }
            }
        }
        beams = std::move(new_beams);
    }

    long long answer = 0;
    for (auto [beam, count] : beams) {
        answer += count;
    }
    std::cout << answer << std::endl;

    return 0;
}