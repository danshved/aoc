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
using Edge = std::tuple<std::string, std::string>;

int main() {
    std::unordered_set<Vertex> vertices;
    std::unordered_map<Edge, int, TupleHasher> edges;

    for (const std::string& line : Split(Trim(GetContents("input.txt")), "\n")) {
        auto [a, b, c] = SplitN(line, " to ", " = ");
        int w = std::stoi(c);
        edges[std::make_pair(a, b)] = w;
        edges[std::make_pair(b, a)] = w;
        vertices.insert(a);
        vertices.insert(b);
    }

    int answer = std::numeric_limits<int>::min();
    std::vector<Vertex> perm(vertices.begin(), vertices.end());
    std::ranges::sort(perm);
    do {
        int dist = 0;
        for (int i = 0; i + 1 < perm.size(); i++) {
            dist += edges.at(std::make_pair(perm[i], perm[i + 1]));
        }
        answer = std::max(answer, dist);
    } while (std::next_permutation(perm.begin(), perm.end()));
    
    std::cout << answer << std::endl;
    return 0;
}