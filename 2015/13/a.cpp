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

using Pair = std::tuple<std::string, std::string>;

int main() {
    std::unordered_set<std::string> names;
    std::unordered_map<Pair, int, TupleHasher> edges;

    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), "\n");
    for (const std::string& line : input) {
        // Alice would gain 54 happiness units by sitting next to Bob.
        auto [from, verb, num, to, _] = SplitN(line, " would ", " ", " happiness units by sitting next to ", ".");
        assert(verb == "gain" || verb == "lose");
        int w = std::stoi(num) * ((verb == "gain") ? 1 : -1);
        edges[{from, to}] = w;
        names.insert(from);
        names.insert(to);
    }

    int answer = std::numeric_limits<int>::min();
    std::vector<std::string> perm(names.begin(), names.end());
    std::ranges::sort(perm);
    do {
        int change = 0;
        for (int i = 0; i < perm.size(); i++) {
            int j = (i + 1) % perm.size();
            change += edges[{perm[i], perm[j]}];
            change += edges[{perm[j], perm[i]}];
        }
        answer = std::max(answer, change);
    } while (std::next_permutation(perm.begin(), perm.end()));

    std::cout << answer << std::endl;
    return 0;
}