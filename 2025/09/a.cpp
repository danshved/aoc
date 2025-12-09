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
    std::vector<Coord> points;
    for (const std::string& line : Split(Trim(GetContents("input.txt")), "\n")) {
        auto [a, b] = SplitN(line, ",");
        points.push_back({std::stoi(a), std::stoi(b)});
    }

    long long answer = 0;
    for (int i = 0; i < points.size(); i++) {
        for (int j = i + 1; j < points.size(); j++) {
            Coord diff = points[i] - points[j];
            long long area = (1 + abs(diff.i)) * static_cast<long long>(1 + abs(diff.j));
            answer = std::max(answer, area);
        }
    }
    std::cout << answer << std::endl;

    return 0;
}