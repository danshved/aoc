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
    int answer = 0;
    int cur = 50;
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), "\n");
    for (const std::string& line: input) {
        int sign = (line[0] == 'R' ? 1 : (-1));
        int amount = std::stoi(line.substr(1));
        for (int i = 0; i < amount; i++) {
            cur = SafeMod(cur + sign, 100);
            if (cur == 0) {
                answer++;
            }
        }

    }
    std::cout << answer << std::endl;

    return 0;
}