#include <iostream>
#include <ranges>
#include <string>
#include <vector>

#include "parse.h"

struct Pair {
    long long from;
    long long to;

    std::strong_ordering operator<=>(const Pair&) const = default;
};

Pair ParsePair(const std::string& s) {
    auto [l, r] = SplitN(s, "-");
    return {std::stoll(l), std::stoll(r)};
}

int main() {
    std::vector<Pair> ranges;
    for (const std::string& line : Split(Trim(GetContents("input.txt")), "\n")) {
        ranges.push_back(ParsePair(line));
    }

    std::ranges::sort(ranges);
    long long i = 0;
    long long answer = 0;
    for (const Pair& range : ranges) {
        if (i < range.from) {
            answer += range.from - i;
        }
        i = std::max(i, range.to + 1);
    }
    answer += (1ll << 32) - i;
    std::cout << answer << std::endl;
    return 0;
}