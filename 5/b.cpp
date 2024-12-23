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
#include "numbers.h"
#include "order.h"
#include "parse.h"

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), '\n');
    auto [top, bottom] = Split2(lines, std::string());

    std::unordered_map<int, std::vector<int>> edges;
    for (const std::string& s : top) {
        auto [l, r] = Split2(s, '|');
        edges[std::stoi(l)].push_back(std::stoi(r));
    }

    int answer = 0;
    for (const std::string& s : bottom) {
        std::vector<int> pages = ParseVector<int>(s);
        std::unordered_map<int, int> pos;
        for (int i = 0; i < pages.size(); i++) {
            pos[pages[i]] = i;
        }

        std::vector<int> order;
        bool ok = true;
        DFS<int>(
            [&](auto& search) {
                for (int u : pages) {
                    search.Look(u);
                }
            },
            [&](auto& search, int u) {
                for (int v : edges[u]) {
                    if (pos.contains(v)) {
                        search.Look(v);
                        ok = ok && pos[u] < pos[v];
                    }
                }
                order.push_back(u);
            });

        if (!ok) {
            answer += order[order.size() / 2];
        }
    }

    std::cout << answer << std::endl;
    return 0;
}