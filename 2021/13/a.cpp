#include <iostream>
#include <unordered_set>

#include "grid.h"
#include "parse.h"

int main() {
    auto [top, bottom] = Split2(Split(Trim(GetContents("input.txt")), "\n"), {""});

    std::unordered_set<Coord> points;
    for (const std::string& line : top) {
        auto [l, r] = SplitN(line, ",");
        int i = std::stoi(l), j = std::stoi(r);
        if (i > 655) {
            i = 2 * 655 - i;
        }
        points.insert({i, j});
    }
    std::cout << points.size() << std::endl;
    return 0;
}