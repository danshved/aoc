#include <iostream>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

#include "parse.h"

int main() {
    std::vector<std::pair<long long, long long>> ranges;
    for (const std::string& line : Split(Trim(GetContents("input.txt")), "\n")) {
        auto [l, r] = SplitN(line, "-");
        ranges.emplace_back(std::stoll(l), std::stoll(r));
    }
    
    std::ranges::sort(ranges);
    long long i = 0;
    long long answer = 0;
    for (const std::pair<long long, long long>& range : ranges) {
        if (i < range.first) {
            answer += range.first - i;
        }
        i = std::max(i, range.second + 1);
    }
    answer += (1ll << 32) - i;
    std::cout << answer << std::endl;
    return 0;
}