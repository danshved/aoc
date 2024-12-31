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

    int answer = 0;
    for (Coord start : box) {
        int score = 1;
        for (Coord dir : Adj4({0, 0})) {
            int count = 0;
            for (Coord cur = start + dir; box.contains(cur); cur += dir) {
                count++;
                if (input[cur.i][cur.j] >= input[start.i][start.j]) {
                    break;
                }
            }
            score *= count;
        }
        answer = std::max(answer, score);
    }

    std::cout << answer << std::endl;
    return 0;
}