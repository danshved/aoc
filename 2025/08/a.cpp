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
    long long x, y, z;

    bool operator==(const Node& other) const = default;
};

template<>
struct std::hash<Node> {
    size_t operator()(const Node& u) const {
        return SeqHash(u.x, u.y, u.z);
    }
};

long long GetDistSquare(const Node& a, const Node& b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
}

struct Edge {
    long long dist_square;
    Node a, b;
    
    auto operator<=>(const Edge& other) const {
        return dist_square <=> other.dist_square;
    }

    bool operator==(const Edge& other) const = default;
};

int main() {
    std::vector<Node> nodes;
    for (const std::string& line : Split(Trim(GetContents("input.txt")), "\n")) {
        auto [x, y, z] = SplitN(line, ",", ",");
        nodes.push_back({std::stoll(x), std::stoll(y), std::stoll(z)});
    }

    std::vector<Edge> edges;
    for (int i = 0; i < nodes.size(); i++) {
        for (int j = i + 1; j < nodes.size(); j++) {
            edges.push_back({GetDistSquare(nodes[i], nodes[j]),nodes[i],nodes[j]});
        }
    }
    std::sort(edges.begin(), edges.end());

    std::unordered_map<Node, std::vector<Node>> graph;
    for (int i = 0; i < 1000; i++) {
        const Edge& e = edges[i];
        graph[e.a].push_back(e.b);
        graph[e.b].push_back(e.a);
    }
    
    std::unordered_map<Node, long long> sizes;
    std::vector<long long> comp_sizes;
    DFS<Node>(
        [&](auto& search) {
            for (const Node& u : nodes) {
                if (search.Look(u) == DFSEdge::kTree) {
                    comp_sizes.push_back(sizes[u]);
                }
            }
        },
        [&](auto& search, const Node& u) {
            sizes[u] = 1;
            for (const Node& v : graph[u]) {
                if (search.Look(v) == DFSEdge::kTree) {
                    sizes[u] += sizes[v];
                }
            }
        }
    );

    std::sort(comp_sizes.begin(), comp_sizes.end());
    std::reverse(comp_sizes.begin(), comp_sizes.end());
    std::cout << (comp_sizes[0] * comp_sizes[1] * comp_sizes[2]) << std::endl;
    return 0;
}