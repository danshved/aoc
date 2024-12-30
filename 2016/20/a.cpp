#include <iostream>
#include <ranges>
#include <string>
#include <vector>

#include "parse.h"

struct Pair {
    unsigned int from;
    unsigned int to;

    std::strong_ordering operator<=>(const Pair&) const = default;
};

Pair ParsePair(const std::string& s) {
    auto [l, r] = SplitN(s, "-");
    return {(unsigned int)std::stoll(l), (unsigned int)std::stoll(r)};
}

int main() {
    std::vector<Pair> ranges;
    for (const std::string& line : Split(Trim(GetContents("input.txt")), "\n")) {
        ranges.push_back(ParsePair(line));
    }

    std::ranges::sort(ranges);
    unsigned int i = 0;
    for (const Pair& range : ranges) {
        if (i < range.from) {
            break;
        }
        i = std::max(i, range.to + 1);
    }
    std::cout << i << std::endl;
    return 0;
}