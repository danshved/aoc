#include <iostream>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

#include "parse.h"

int main() {
    std::vector<std::pair<unsigned int, unsigned int>> ranges;
    for (const std::string& line : Split(Trim(GetContents("input.txt")), "\n")) {
        auto [l, r] = SplitN(line, "-");
        ranges.emplace_back((unsigned int)std::stoll(l), (unsigned int)std::stoll(r));
    }

    std::ranges::sort(ranges);
    unsigned int i = 0;
    for (const std::pair<unsigned int, unsigned int>& range : ranges) {
        if (i < range.first) {
            break;
        }
        i = std::max(i, range.second + 1);
    }
    std::cout << i << std::endl;
    return 0;
}