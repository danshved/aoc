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

struct Node {
    std::string name;
    bool has_dac = false;
    bool has_fft = false;

    bool operator==(const Node& other) const = default;
};

template<>
struct std::hash<Node> {
    size_t operator()(const Node& u) const {
        return SeqHash(u.name, u.has_dac, u.has_fft);
    }
};

int main() {
    std::unordered_map<std::string, std::vector<std::string>> graph;
    for (const std::string& line : Split(Trim(GetContents("input.txt")), "\n")) {
        auto [from, right] = SplitN(line, ": ");
        std::vector<std::string>& edges = graph[from];
        for (const std::string& to : Split(right, " ")) {
            edges.push_back(to);
        }
    }

    std::unordered_map<Node, long long> counts;
    const Node kStart{"svr", false, false};
    const Node kFinish{"out", true, true};
    DFSFrom(kStart, [&](auto& search, const Node& u) {
        if (u == kFinish) {
            counts[u] = 1;
            return;
        }
        counts[u] = 0;
        for (const std::string& name : graph[u.name]) {
            Node v{name, u.has_dac || name == "dac", u.has_fft || name == "fft"};
            search.Look(v);
            counts[u] += counts[v];
        }
    });

    std::cout << counts[kStart] << std::endl;
    return 0;
}