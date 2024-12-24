#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
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

int main() {
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), '\n');
    auto [size_i, size_j] = Sizes<2>(input);
    PosDir start = {FindOrDie<2>(input, 'S'), kEast};
    PosDir end = {FindOrDie<2>(input, 'E'), kNorth};

    auto visit = [&](auto& search, const PosDir& u, int d) {
        search.Look(u.RotateRight(), d + 1000);
        search.Look(u.RotateLeft(), d + 1000);
        PosDir v = u.Step();
        if (InBounds(v.pos, size_i, size_j) && input[v.pos.i][v.pos.j] != '#') {
            search.Look(v, d + 1);
        }
    };
    std::unordered_map<PosDir, int> from_start = DijkstraFrom(start, 0, visit);
    std::unordered_map<PosDir, int> from_end = DijkstraFrom(end.Flip(), 0, visit);

    // Vertex X is on a shortest path from S to E if and only if
    // dist(S, X) + dist(X, E) = dist(S, E).
    int answer = std::ranges::count_if(Bounds(size_i, size_j), [&](Coord pos) {
        return std::ranges::any_of(kDirs, [&](Coord dir) {
            PosDir x = {pos, dir};
            return from_start.contains(x) && from_end.contains(x.Flip()) &&
                   from_start[x] + from_end[x.Flip()] == from_start[end];
        });
    });
    std::cout << answer << std::endl;
    return 0;
}