#include <iostream>
#include <string>
#include <vector>
#include <ranges>

#include "collections.h"
#include "grid.h"
#include "parse.h"

const char kXmas[] = "XMAS";

int main() {
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), "\n");
    auto [size_i, size_j] = Sizes<2>(input);
    auto is_xmas = [&](Coord start, Coord dir) {
        return std::ranges::all_of(std::ranges::iota_view(0, 4), [&](int k) {
            Coord c = start + k * dir;
            return (InBounds(c, size_i, size_j) && input[c.i][c.j] == kXmas[k]);
        });
    };

    int answer = 0;
    for (Coord start : Bounds(size_i, size_j)) {
        answer += std::ranges::count_if(Adj8({0, 0}), [&](Coord dir) {
            return is_xmas(start, dir);
        });
    }

    std::cout << answer << std::endl;
    return 0;
}