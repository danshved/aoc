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

const std::unordered_map<char, std::string> kSubst =
    {{'#', "##"}, {'.', ".."}, {'O', "[]"}, {'@', "@."}};

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), '\n');
    auto [pre_matrix, bottom] = Split2(lines, std::string());
    std::string moves = Concat(std::move(bottom));

    // Transform the matrix for part 2.
    std::vector<std::string> matrix;
    for (const std::string& s : pre_matrix) {
        matrix.push_back(ConcatMap(s, [](char c) {
            return kSubst.at(c);
        }));
    }
    auto [size_i, size_j] = Sizes<2>(matrix);
    auto second_half = [&](Coord c) {
        assert(matrix[c.i][c.j] == '[' || matrix[c.i][c.j] == ']');
        return (matrix[c.i][c.j] == '[') ? Coord{c.i, c.j + 1} : Coord{c.i, c.j - 1};
    };

    Coord robot = FindOrDie<2>(matrix, '@');
    for (char c : moves) {
        Coord dir = kCharToDir.at(c);

        // Build the list of all cells to move. The list will be ordered so that
        // each cell comes before its blockers.
        std::vector<Coord> to_move;
        bool stuck = false;
        BFSFrom(robot, [&](auto& search, const Coord& pusher) {
            Coord next = pusher + dir;
            if (!InBounds(next, size_i, size_j) || matrix[next.i][next.j] == '#') {
                stuck = true;
                return;
            }
            if (matrix[next.i][next.j] != '.') {
                search.Look(next);
                if (dir.i != 0) {
                    search.Look(second_half(next));
                }
            }
            to_move.push_back(pusher);
        });

        // Now move everything that needs to be moved.
        if (!stuck) {
            for (Coord c : to_move | std::views::reverse) {
                Coord next = c + dir;
                matrix[next.i][next.j] = std::exchange(matrix[c.i][c.j], '.');
            }
            robot += dir;
        }
    }

    int answer = 0;
    for (auto [i, j] : Bounds(size_i, size_j)) {
        if (matrix[i][j] == '[') {
            answer += 100 * i + j;
        }
    }
    std::cout << answer << std::endl;
    return 0;
}