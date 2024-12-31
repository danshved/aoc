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

const std::unordered_map<Coord, int> kDirCodes = {
    {kEast, 0}, {kSouth, 1}, {kWest, 2}, {kNorth, 3}};

int main() {
    std::unordered_map<Coord, char> map;
    Box box;

    std::vector<std::string> lines = Split(GetContents("input.txt"), "\n");
    auto [matrix, bottom] = Split2(lines, {""});
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            if (matrix[i][j] != ' ') {
                map[{i, j}] = matrix[i][j];
                box.size_i = std::max(box.size_i, i + 1);
                box.size_j = std::max(box.size_j, j + 1);
            }
        }
    }
    Coord start = {0, (int)matrix[0].find('.')};
    assert(start.j != std::string::npos);

    assert(bottom.size() == 1);
    std::string commands = bottom[0];

    auto step = [&](const PosDir& u) {
        PosDir v = u.Step();
        if (!map.contains(v.pos)) {
            for (PosDir x = u; box.contains(x.pos); x = x.StepBack()) {
                if (map.contains(x.pos)) {
                    v = x;
                }
            }
        }
        return (map[v.pos] == '#') ? u : v;
    };

    PosDir state = {start, kEast};
    auto end = commands.end();
    for (auto it = commands.begin(); it != end;) {
        if (*it == 'R') {
            state = state.RotateRight();
            it++;
        } else if (*it == 'L') {
            state = state.RotateLeft();
            it++;
        } else {
            auto next = std::find_if(it, end, [](char c) {
                return c == 'L' || c == 'R';
            });
            int steps = std::stoi(std::string(it, next));
            for (int i = 0; i < steps; i++) {
                state = step(state);
            }
            it = next;
        }
    }

    int answer = 1000 * (state.pos.i + 1) + 4 * (state.pos.j + 1) + kDirCodes.at(state.dir);
    std::cout << answer << std::endl;
    return 0;
}