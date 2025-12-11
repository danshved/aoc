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
    std::unordered_map<std::string, std::vector<std::string>> graph;
    for (const std::string& line : Split(Trim(GetContents("input.txt")), "\n")) {
        auto [from, right] = SplitN(line, ": ");
        auto& edges = graph[from];
        for (const std::string& to : Split(right, " ")) {
            edges.push_back(to);
        }
    }

    std::unordered_map<std::string, long long> counts;
    DFSFrom(std::string{"you"}, [&](auto& search, const std::string& u) {
        if (u == "out") {
            counts[u] = 1;
            return;
        }
        counts[u] = 0;
        for (const std::string& v : graph[u]) {
            search.Look(v);
            counts[u] += counts[v];
        }
    });

    std::cout << counts["you"] << std::endl;
    return 0;
}