#include <iostream>
#include <string>
#include <vector>

#include "collections.h"
#include "grid.h"
#include "parse.h"

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), "\n");
    auto [matrix, bottom] = Split2(lines, {""});
    std::string moves = Concat(std::move(bottom));
    Box box = Sizes<2>(matrix);
    Coord pos = FindOrDie<2>(matrix, '@');

    matrix[pos.i][pos.j] = '.';
    for (char c : moves) {
        Coord dir = kDirArrows.at(c), end = pos + dir;
        while(box.contains(end) && matrix[end.i][end.j] == 'O') {
            end += dir;
        }
        if (!box.contains(end) || matrix[end.i][end.j] == '#') {
            continue;
        }

        pos += dir;
        matrix[end.i][end.j] = 'O';
        matrix[pos.i][pos.j] = '.';
    }

    int answer = 0;
    for (auto [i, j] : box) {
        if (matrix[i][j] == 'O') {
            answer += 100 * i + j;
        }
    }
    std::cout << answer << std::endl;
    return 0;
}