#include <iostream>
#include <ranges>
#include <string>
#include <vector>

#include "collections.h"
#include "grid.h"
#include "parse.h"

int main() {
    std::vector<std::string> matrix = Split(Trim(GetContents("input.txt")), "\n");
    Box box = Sizes<2>(matrix);

    while (true) {
        std::vector<std::string> new_matrix(box.size_i, std::string(box.size_j, '.'));
        for (Coord u : box) {
            int count = std::ranges::count_if(Adj8(u), [&](Coord v) {
                return box.contains(v) && matrix[v.i][v.j] == '#';
            });
            new_matrix[u.i][u.j] =
                (matrix[u.i][u.j] == 'L' && count == 0)   ? '#'
                : (matrix[u.i][u.j] == '#' && count >= 4) ? 'L'
                                                          : matrix[u.i][u.j];
        }
        if (matrix != new_matrix) {
            matrix = std::move(new_matrix);
        } else {
            break;
        }
    }

    int answer = std::ranges::count_if(box, [&](Coord u) {
        return matrix[u.i][u.j] == '#';
    });
    std::cout << answer << std::endl;
    return 0;
}