#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <ranges>

#include "collections.h"
#include "grid.h"
#include "numbers.h"
#include "order.h"
#include "parse.h"

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), '\n');
    auto [matrix, bottom] = Split2(lines, std::string());
    std::string moves = Concat(bottom);
    auto [size_i, size_j] = Sizes<2>(matrix);
    Coord pos = FindOrDie<2>(matrix, '@');

    matrix[pos.i][pos.j] = '.';
    for (char c : moves) {
        Coord dir = kCharToDir.at(c), end = pos + dir;
        while(InBounds(end, size_i, size_j) && matrix[end.i][end.j] == 'O') {
            end += dir;
        }
        if (!InBounds(end, size_i, size_j) || matrix[end.i][end.j] == '#') {
            continue;
        }

        pos += dir;
        matrix[end.i][end.j] = 'O';
        matrix[pos.i][pos.j] = '.';
    }

    int answer = 0;
    for (auto [i, j] : Bounds(size_i, size_j)) {
        if (matrix[i][j] == 'O') {
            answer += 100 * i + j;
        }
    }
    std::cout << answer << std::endl;
    return 0;
}