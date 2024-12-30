#include <iostream>
#include <limits>
#include <unordered_map>
#include <vector>

#include "collections.h"
#include "graph_search.h"
#include "grid.h"
#include "parse.h"

int main() {
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), "\n");
    Box box = Sizes<2>(input);
    std::vector<Coord> points = {Coord(FindOrDie<2>(input, '0'))};
    for (int i = 1; i <= 9; i++) {
        if (auto x = Find<2>(input, '0' + i); x.has_value()) {
            points.emplace_back(*x);
        }
    }

    std::unordered_map<Coord, std::unordered_map<Coord, int>> dist;
    for (Coord start : points) {
        DijkstraFrom(start, 0, [&](auto& search, Coord u, int d) {
            if (isdigit(input[u.i][u.j])) {
                dist[start][u] = d;
            }
            for (Coord v : Adj4(u)) {
                if (box.contains(v) && input[v.i][v.j] != '#') {
                    search.Look(v, d + 1);
                }
            }
        });
    }

    int answer = std::numeric_limits<int>::max();
    std::sort(points.begin() + 1, points.end());
    do {
        int total = 0;
        for (int i = 0; i < points.size(); i++) {
            total += dist.at(points[i]).at(points[(i + 1) % points.size()]);
        }
        answer = std::min(answer, total);
    } while (std::next_permutation(points.begin() + 1, points.end()));

    std::cout << answer << std::endl;
    return 0;
}