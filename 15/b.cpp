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
#include "numbers.h"
#include "order.h"
#include "parse.h"

struct Coord {
    int i;
    int j;
};

const std::unordered_map<char, std::string> kSubst =
    {{'#', "##"}, {'.', ".."}, {'O', "[]"}, {'@', "@."}};

const std::unordered_map<char, Coord> kDirs =
    {{'^', {-1, 0}}, {'v', {1, 0}}, {'<', {0, -1}}, {'>', {0, 1}}};

std::vector<std::string> matrix;
int height, width;

bool InBounds(Coord c) {
    return c.i >= 0 && c.i < height && c.j >= 0 && c.j < width;
}

Coord SecondHalf(Coord c) {
    assert(matrix[c.i][c.j] == '[' || matrix[c.i][c.j] == ']');
    return (matrix[c.i][c.j] == '[') ? Coord{c.i, c.j + 1} : Coord{c.i, c.j - 1};
}

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), '\n');
    auto [pre_matrix, bottom] = Split2(lines, std::string());
    std::string moves = Concat(bottom);

    // Transform the matrix for part 2.
    for (const std::string& s : pre_matrix) {
        std::ostringstream oss;
        for (char c : s) {
            oss << kSubst.at(c);
        }
        matrix.push_back(oss.str());
    }
    height = matrix.size();
    width = matrix[0].size();

    // Locate the robot.
    Coord robot;
    for (robot.i = 0; robot.i < height; robot.i++) {
        robot.j = matrix[robot.i].find('@');
        if (robot.j != std::string::npos) {
            break;
        }
    }
    assert(robot.i < height);
    matrix[robot.i][robot.j] = '.';

    for (char c : moves) {
        Coord d = kDirs.at(c);

        // BFS bookkeeping.
        std::queue<Coord> q;
        std::vector<std::vector<bool>> added(height, std::vector<bool>(width, false));

        // BFS output.
        std::vector<Coord> to_move;
        bool stuck = false;

        // Breadth first search to build the list of all cells to_move. The
        // list will be ordered so that each cell comes before its blockers.
        q.push(robot);
        added[robot.i][robot.j] = true;
        while (!q.empty()) {
            Coord pusher = q.front();
            q.pop();

            // See where the current cell ("the pusher") wants to move.
            Coord next = {pusher.i + d.i, pusher.j + d.j};
            if (!InBounds(next) || matrix[next.i][next.j] == '#') {
                stuck = true;
                break;
            }
            if (matrix[next.i][next.j] == '.') {
                continue;
            }

            // Build the list of cells that the pusher is pushing. It's pushing
            // one cell if moving horizontally and two cells ("[]") if moving
            // vertically.
            std::vector<Coord> pushees = {next};
            if (d.i != 0) {
                pushees.push_back(SecondHalf(next));
            }

            // Add the cells being pushed to the queue.
            for (const Coord& pushee : pushees) {
                if (!added[pushee.i][pushee.j]) {
                    q.push(pushee);
                    added[pushee.i][pushee.j] = true;
                    to_move.push_back(pushee);
                }
            }
        }

        // Now move everything that needs to be moved at once.
        if (!stuck) {
            for (const auto& [i, j] : to_move | std::views::reverse) {
                matrix[i + d.i][j + d.j] = std::exchange(matrix[i][j], '.');
            }
            robot.i += d.i;
            robot.j += d.j;
        }
    }

    // Compute the answer.
    int answer = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (matrix[i][j] == '[') {
                answer += 100 * i + j;
            }
        }
    }

    std::cout << answer << std::endl;
    return 0;
}