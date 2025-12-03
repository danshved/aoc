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
    for (const std::string& line : Split(Trim(GetContents("input.txt")), "\n")) {
        int best = 0;
        for (int i = 0; i < line.size(); i++) {
            for (int j = i + 1; j < line.size(); j++) {
                int a = line[i] - '0';
                int b = line[j] - '0';
                best = std::max(best, 10 * a + b);
            }
        }
        answer += best;
    }
    std::cout << answer << std::endl;

    return 0;
}