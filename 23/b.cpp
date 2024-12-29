#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "collections.h"
#include "parse.h"

int main() {
    std::unordered_map<std::string, std::unordered_set<std::string>> edges;
    for (const std::string& line : Split(Trim(GetContents("input.txt")), "\n")) {
        auto [l, r] = SplitN(line, "-");
        edges[l].insert(r);
        edges[r].insert(l);
    }

    auto is_clique = [&](const std::vector<std::string> group) {
        for (int i = 0; i < group.size(); i++) {
            for (int j = i + 1; j < group.size(); j++) {
                if(!edges[group[i]].contains(group[j])) {
                    return false;
                }
            }
        }
        return true;
    };

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
            if (selected.size() + 1 > best_count && is_clique(selected)) {
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