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

struct State {
    PosDir pd;
    int steps = 0;

    bool operator==(const State&) const = default;
};

template <>
struct std::hash<State> {
    size_t operator()(const State& s) const {
        return SeqHash(s.pd, s.steps);
    }
};

int main() {
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), "\n");
    Box box = Sizes<2>(input);

    int answer = std::numeric_limits<int>::max();
    Dijkstra<State, int, std::hash<State>, ShortQueue<State>>(
        [&](auto& search) {
            search.Look({{{0, 0}, kEast}, 0}, 0);
            search.Look({{{0, 0}, kSouth}, 0}, 0);
        },
        [&](auto& search, const State& u, int d) {
            if (u.pd.pos == Coord{box.size_i - 1, box.size_j - 1}) {
                answer = std::min(answer, d);
                return;
            }
            
            if (u.steps < 10) {
                State v = {u.pd.Step(), u.steps + 1};
                if (box.contains(v.pd.pos)) {
                    search.Look(v, d + (input[v.pd.pos.i][v.pd.pos.j] - '0'));
                }
            }
            if (u.steps >= 4) {
                for (Coord dir : {u.pd.dir.RotateRight(), u.pd.dir.RotateLeft()}) {
                    search.Look({{u.pd.pos, dir}, 0}, d);
                }
            }
        });

    std::cout << answer << std::endl;
    return 0;
}