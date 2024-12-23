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

    std::unordered_set<NTuple<3, std::string>, TupleHasher> seen;
    int answer = 0;
    for (const auto& [a, a_out] : edges) {
        if (a[0] != 't') {
            continue;
        }
        for (const std::string& b : a_out) {
            for (const std::string& c : a_out) {
                if (c <= b) {
                    continue;
                }
                if (!edges[b].contains(c)) {
                    continue;
                }

                std::vector<std::string> v = {a, b, c};
                std::sort(v.begin(), v.end());
                NTuple<3, std::string> t = {v[0], v[1], v[2]};
                if (!seen.contains(t)) {
                    seen.insert(t);
                    answer++;
                }
            }
        }
    }

    std::cout << answer << std::endl;
    return 0;
}