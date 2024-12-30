#include <iostream>
#include <ranges>
#include <string>
#include <unordered_map>

#include "grid.h"
#include "parse.h"

int main() {
    std::unordered_map<Coord, int> counts;
    for (const std::string& line : Split(Trim(GetContents("input.txt")), "\n")) {
        auto [a, b, c, d] = SplitN(line, ",", " -> ", ",");
        Coord from = {std::stoi(a), std::stoi(b)};
        Coord to = {std::stoi(c), std::stoi(d)};
        if (from.i != to.i && from.j != to.j) {
            continue;
        }
        for (Coord c : PathCC(from, to)) {
            counts[c]++;
        }
    }

    int answer = std::ranges::count_if(counts | std::views::values, [](int count) {
        return count >= 2;
    });
    std::cout << answer << std::endl;
    return 0;
}