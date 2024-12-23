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
#include "numbers.h"
#include "order.h"
#include "parse.h"

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), '\n');
    std::unordered_map<std::string, std::unordered_set<std::string>> edges;
    for (const std::string& line : lines) {
        auto [l, r] = Split2(line, '-');
        edges[l].insert(r);
        edges[r].insert(l);
    }

    int best_count = 0;
    std::vector<std::string> best_clique;
    for (const auto& [a, a_out] : edges) {
        std::vector<std::string> v(a_out.begin(), a_out.end());
        for (unsigned int mask = 0; mask < (1u << v.size()); mask++) {
            std::vector<std::string> selected;
            for (int i = 0; i < v.size(); i++) {
                if (mask & (1u << i)) {
                    selected.push_back(v[i]);
                }
            }
            if (selected.size() + 1 <= best_count) {
                continue;
            }

            bool ok = true;
            for (int i = 0; i < selected.size() && ok; i++) {
                for (int j = i + 1; j < selected.size() && ok; j++) {
                    ok = ok && edges[selected[i]].contains(selected[j]);
                }
            }

            if (ok) {
                best_count = 1 + selected.size();
                best_clique = selected;
                best_clique.push_back(a);
            }
        }
    }

    std::sort(best_clique.begin(), best_clique.end());
    std::cout << FormatVector(best_clique, ",") << std::endl;
    return 0;
}