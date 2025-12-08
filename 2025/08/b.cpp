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

    std::unordered_map<Node, std::vector<Node>> graph;
    auto is_connected = [&]() {
        int visited = 0;
        DFSFrom(nodes[0], 
            [&](auto& search, const Node& u) {
                visited++;
                for (const Node& v : graph[u]) {
                    search.Look(v);
                }
            });
        return (visited == nodes.size());
    };

    std::vector<Edge> edges;
    for (int i = 0; i < nodes.size(); i++) {
        for (int j = i + 1; j < nodes.size(); j++) {
            edges.push_back({GetDistSquare(nodes[i], nodes[j]), nodes[i], nodes[j]});
        }
    }
    std::sort(edges.begin(), edges.end());

    for (const Edge& e : edges) {
        graph[e.a].push_back(e.b);
        graph[e.b].push_back(e.a);
        if (is_connected()) {
            std::cout << e.a.x * e.b.x << std::endl;
            break;
        }
    }
    
    return 0;
}