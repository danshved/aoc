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

    int answer = 0;
    std::unordered_set<int> beams = {start.j};
    for (int i = 0; i < box.size_i - 1; i++) {
        std::cout << "row " << i << std::endl;
        std::unordered_set<int> new_beams;
        for (int beam : beams) {
            if (input[i + 1][beam] == '.') {
                new_beams.insert(beam);
            } else {
                answer++;
                if (beam - 1 >= 0) {
                    new_beams.insert(beam - 1);
                }
                if (beam + 1 < box.size_j) {
                    new_beams.insert(beam + 1);
                }
            }
        }
        beams = std::move(new_beams);
    }
    std::cout << answer << std::endl;

    return 0;
}