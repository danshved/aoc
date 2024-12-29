#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <optional>
#include <ranges>
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

bool BeginsWith(const std::string& a, const std::string& b) {
    if (a.size() < b.size()) {
        return false;
    }
    return std::equal(b.begin(), b.end(), a.begin());
}

int main() {
    std::vector<std::string> pixels(6, std::string(50, ' '));
    Box box = Sizes<2>(pixels);
    int step = 0;
    for (const std::string& line : Split(Trim(GetContents("input.txt")), "\n")) {
        if (BeginsWith(line, "rect")) {
            auto [_, j, i] = SplitN(line, "rect ", "x");
            for (Coord c : Box(std::stoi(i), std::stoi(j))) {
                pixels[c.i][c.j] = '#';
            }
        } else {
            PathCO path;
            Coord delta;
            if (BeginsWith(line, "rotate row")) {
                auto [_, i, d] = SplitN(line, "rotate row y=", " by ");
                path = PathCO({std::stoi(i), 0}, {std::stoi(i), box.size_j});
                delta = Coord{0, std::stoi(d)};
            } else {
                auto [_, j, d] = SplitN(line, "rotate column x=", " by ");
                path = PathCO({0, std::stoi(j)}, {box.size_i, std::stoi(j)});
                delta = Coord{std::stoi(d), 0};
            }
            DFSFrom<Coord>(path, [&](auto& search, Coord u) {
                Coord v = box.Wrap(u + delta);
                if (search.Look(v) == DFSEdge::kTree) {
                    std::swap(pixels[u.i][u.j], pixels[v.i][v.j]);
                }
            });
        }
    }

    std::cout << FormatVector(pixels, "\n") << std::endl;
    return 0;
}