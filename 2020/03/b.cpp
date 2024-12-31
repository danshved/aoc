#include <iostream>
#include <ranges>
#include <string>
#include <vector>

#include "collections.h"
#include "grid.h"
#include "parse.h"

const Coord kDirs[5] = {{1, 1}, {1, 3}, {1, 5}, {1, 7}, {2, 1}};

int main() {
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), "\n");
    Box box = Sizes<2>(input);

    long long answer = 1;
    for (Coord dir : kDirs) {
        answer *= std::ranges::count_if(std::views::iota(1, box.size_i / dir.i), [&](int k) {
            Coord c = box.Wrap(k * dir);
            return input[c.i][c.j] == '#';
        });
    }
    std::cout << answer << std::endl;
    return 0;
}