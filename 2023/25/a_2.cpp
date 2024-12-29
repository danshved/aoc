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
#include <random>

#include "collections.h"
#include "graph_search.h"
#include "grid.h"
#include "numbers.h"
#include "order.h"
#include "parse.h"

using Vertex = std::string;
using Edge = std::pair<Vertex, Vertex>;

template<>
struct std::hash<Edge> {
    size_t operator()(const Edge& e) const {
        return SeqHash(e.first, e.second);
    }
};

template <typename T>
using PerVertex = std::unordered_map<std::string, T>;

Edge Sort(const Edge& e) {
    return (e.first < e.second) ? e : Edge{e.second, e.first};
}

int main() {
    PerVertex<std::vector<Vertex>> graph;
    std::vector<Edge> edges;
    for (const std::string& s : Split(Trim(GetContents("input.txt")), '\n')) {
        auto [u, right] = Split2(s, ':');
        for (const std::string& v : Split(Trim(right), ' ')) {
            graph[u].push_back(v);
            graph[v].push_back(u);
            edges.push_back({u, v});
        }
    }

    std::vector<Vertex> vertices;
    std::ranges::copy(graph | std::views::keys, std::back_inserter(vertices));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(0, graph.size());
    std::unordered_map<Edge, int> counts;
    for (int i = 0; i < 1000; i++) {
        Vertex from = vertices[distrib(gen)];
        Vertex to = vertices[distrib(gen)];
        PerVertex<std::optional<Vertex>> parent;
        BFSFrom(from, [&](auto& search, const Vertex& u) {
            parent[u] = search.Parent();
            for (const Vertex& v : graph[u]) {
                search.Look(v);
            }
        });
        while (parent[to].has_value()) {
            counts[Sort({to, *parent[to]})]++;
            to = *parent[to];
        }
    }

    std::vector<std::pair<int, Edge>> freqs;
    for (const auto& [k, v] : counts) {
        freqs.push_back({v, k});
    }
    std::ranges::sort(freqs, std::greater<std::pair<int, Edge>>());

    int answer = 1;
    int comp_size = 0;
    DFS<Vertex>(
        [&](auto& search) {
            for (const Vertex& u : graph | std::views::keys) {
                if (search.Look(u) == DFSEdge::kTree) {
                    answer *= comp_size;
                }
            }
        },
        [&](auto& search, const Vertex& u) {
            if (search.Depth() == 0) {
                comp_size = 0;
            }
            comp_size++;
            for (const Vertex& v : graph[u]) {
                Edge e = Sort({u, v});
                if (e == freqs[0].second || e == freqs[1].second || e == freqs[2].second) {
                    continue;
                }
                search.Look(v);
            }
        });
    std::cout << answer << std::endl;
    return 0;
}