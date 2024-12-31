#include <iostream>
#include <ranges>
#include <string>
#include <vector>

#include "collections.h"
#include "grid.h"
#include "parse.h"

int main() {
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), "\n");
    Box box = Sizes<2>(input);

    int answer = std::ranges::count_if(std::views::iota(1, box.size_i), [&](int i) {
        Coord c = box.Wrap({i, 3 * i});
        return input[c.i][c.j] == '#';
    });
    std::cout << answer << std::endl;
    return 0;
}