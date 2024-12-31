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
        auto see = [&](Coord u, Coord dir) -> bool {
            if (matrix[u.i][u.j] == '.') {
                return false;
            }
            for (u += dir; box.contains(u); u += dir) {
                if (matrix[u.i][u.j] == '#') {
                    return true;
                } else if (matrix[u.i][u.j] == 'L') {
                    return false;
                }
            }
            return false;
        };

        std::vector<std::string> new_matrix(box.size_i, std::string(box.size_j, '.'));
        for (Coord u : box) {
            int count = std::ranges::count_if(Adj8({0, 0}), [&](Coord dir) {
                return see(u, dir);
            });
            new_matrix[u.i][u.j] =
                (matrix[u.i][u.j] == 'L' && count == 0)   ? '#'
                : (matrix[u.i][u.j] == '#' && count >= 5) ? 'L'
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