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
    int answer = 0;
    for (const std::string& line : Split(Trim(GetContents("input.txt")), "\n")) {
        auto [a, b, c] = SplitN(line, "x", "x");
        int s[3] = {std::stoi(a), std::stoi(b), std::stoi(c)};
        std::ranges::sort(s);
        answer += 2 * s[0] + 2 * s[1] + s[0] * s[1] * s[2];
    }
    std::cout << answer << std::endl;
    return 0;
}