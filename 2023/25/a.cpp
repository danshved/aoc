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

using Vertex = std::string;
using Edge = std::pair<Vertex, Vertex>;

template <typename T>
using PerVertex = std::unordered_map<std::string, T>;

Edge Flip(const Edge& e) {
    return {e.second, e.first};
}

std::string Format(const Edge& e) {
    return e.first + "-" + e.second;
}

void PrintAnswer(const Edge& a, const Edge& b, const Edge& c, int size1, int size2) {
    std::cout << Format(a) << ", " << Format(b) << ", " << Format(c)
              << ", sizes: " << size1 << ", " << size2
              << ", answer: " << (size1 * size2) << std::endl;
}

int main() {
    PerVertex<std::vector<Vertex>> graph;
    std::vector<Edge> edges;
    for (const std::string& s : Split(Trim(GetContents("input.txt")), "\n")) {
        auto [u, right] = SplitN(s, ":");
        for (const std::string& v : Split(Trim(right), " ")) {
            graph[u].push_back(v);
            graph[v].push_back(u);
            edges.push_back({u, v});
        }
    }

    for (const Edge& ignore : edges) {
        PerVertex<int> depth, back_depth, size, back_count;
        PerVertex<std::optional<Vertex>> parent;
        PerVertex<std::vector<Vertex>> children, back;
        std::optional<Vertex> comp_root = std::nullopt;

        DFSFrom(graph.begin()->first, [&](auto& search, const Vertex& u) {
            depth[u] = search.Depth();
            back_depth[u] = search.Depth();
            size[u] = 1;
            parent[u] = search.Parent();
            for (const Vertex& v : graph[u]) {
                Edge e = {u, v};
                if (e == ignore || e == Flip(ignore) || search.Parent() == v) {
                    continue;
                }

                DFSEdge kind = search.Look(v);
                if (kind == DFSEdge::kTree) {
                    children[u].push_back(v);
                    size[u] += size[v];
                    back_depth[u] = std::min(back_depth[u], back_depth[v]);
                    back_count[u] += back_count[v];
                } else if (kind == DFSEdge::kBack) {
                    back[u].push_back(v);
                    back_depth[u] = std::min(back_depth[u], depth[v]);
                    back_count[u]++;
                } else {
                    assert(kind == DFSEdge::kForward);
                    back_count[u]--;
                }
            };

            if (search.Depth() > 0 && back_count[u] == 1) {
                comp_root = u;
            };
        });

        // This is actually one of two possible cases: when the edge escaping
        // from the component is a back-edge. Lucky for us, this finds the
        // answer.
        //
        // The second case is when the escape edge is another tree-edge below
        // the entry edge. We don't handle this one.
        if (comp_root.has_value()) {
            Edge escape;
            DFSFrom(*comp_root, [&](auto& search, const Vertex& u) {
                for (const Vertex& v : back[u]) {
                    if (depth[v] < depth[*comp_root]) {
                        escape = {u, v};
                    }
                }
                for (const Vertex& v : children[u]) {
                    search.Look(v);
                }
            });
            PrintAnswer(ignore, {*comp_root, *parent[*comp_root]}, escape,
                        size[*comp_root], graph.size() - size[*comp_root]);
            break;
        }
    }
    return 0;
}