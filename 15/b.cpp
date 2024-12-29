#include <iostream>
#include <ranges>
#include <string>
#include <vector>

#include "collections.h"
#include "graph_search.h"
#include "grid.h"
#include "parse.h"

const std::unordered_map<char, std::string> kSubst =
    {{'#', "##"}, {'.', ".."}, {'O', "[]"}, {'@', "@."}};

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), "\n");
    auto [pre_matrix, bottom] = Split2(lines, {""});
    std::string moves = Concat(std::move(bottom));

    std::vector<std::string> matrix;
    for (const std::string& s : pre_matrix) {
        matrix.push_back(ConcatMap(s, [](char c) {
            return kSubst.at(c);
        }));
    }
    Box box = Sizes<2>(matrix);
    Coord robot = FindOrDie<2>(matrix, '@');

    auto second_half = [&](Coord c) {
        assert(matrix[c.i][c.j] == '[' || matrix[c.i][c.j] == ']');
        return (matrix[c.i][c.j] == '[') ? Coord{c.i, c.j + 1} : Coord{c.i, c.j - 1};
    };

    for (char c : moves) {
        Coord dir = kDirArrows.at(c);

        // Build the list of all cells to move. The list will be ordered so that
        // each cell comes before its blockers.
        std::vector<Coord> to_move;
        bool stuck = false;
        BFSFrom(robot, [&](auto& search, const Coord& pusher) {
            Coord next = pusher + dir;
            if (!box.contains(next) || matrix[next.i][next.j] == '#') {
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
    for (auto [i, j] : box) {
        if (matrix[i][j] == '[') {
            answer += 100 * i + j;
        }
    }
    std::cout << answer << std::endl;
    return 0;
}