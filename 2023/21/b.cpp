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

const int kRadius = 26501365;

int main() {
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), '\n');
    Coord start = FindOrDie<2>(input, 'S');
    auto [size_i, size_j] = Sizes<2>(input);
    assert(size_i == size_j);
    int size = size_i;
    assert(size % 2 == 1);

    // Important points through which inter-square shortest paths can always
    // be made to pass.
    std::vector<Coord> hubs;
    for (int i : {0, start.i, size - 1}) {
        for (int j : {0, start.j, size - 1}) {
            hubs.emplace_back(i, j);
        }
    }
    auto opposite = [&](Coord hub) {
        if (hub.i != start.i) {
            hub.i = size - 1 - hub.i;
        }
        if (hub.j != start.j) {
            hub.j = size - 1 - hub.j;
        }
        return hub;
    };

    // Find shortest distances from each hub to everywhere within the zero square.
    std::unordered_map<Coord, std::unordered_map<Coord, int>> d;
    for (Coord hub : hubs) {
        d[hub] = BFSFrom(hub, [&](auto& search, Coord u) {
            for (Coord dir : kDirs) {
                Coord v = u + dir;
                if (InBounds(v, size, size) && input[v.i][v.j] != '#') {
                    search.Look(v);
                }
            }
        });
    }

    long long answer = 0;
    for (Coord hub : hubs) {
        for (Coord pos : Bounds(size, size)) {
            if (!d.at(hub).contains(pos)) {
                continue;
            }
            if (hub == start) {
                // Count this original tile within the zero square.
                int left = kRadius - d[start][pos];
                if (left >= 0 && left % 2 == 0) {
                    answer++;
                }
            } else if (hub.i == start.i || hub.j == start.j) {
                // Count copies in squares (possibly far) above, below, to the
                // right and to the left of the zero square.
                int left = kRadius - d[start][opposite(hub)] - d[hub][pos] - 1;
                if (left >= 0) {
                    if (left % 2 == 0) {
                        answer += (left / size / 2) + 1;
                    } else {
                        answer += (left / size + 1) / 2;
                    }
                }
            } else {
                // Count copies in the rest of the squares, i.e. not in line
                // with the zero square either vertically or horizontally.
                int left = kRadius - d[start][opposite(hub)] - d[hub][pos] - 2;
                if (left >= 0) {
                    if (left % 2 == 0) {
                        long long k = (left / size / 2) + 1;
                        answer += k * k;
                    } else {
                        long long k = (left / size + 1) / 2;
                        answer += k * (k + 1);
                    }
                }
            }
        }
    }

    std::cout << answer << std::endl;
    return 0;
}