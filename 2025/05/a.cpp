#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <optional>
#include <ranges>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "collections.h"
#include "graph_search.h"
#include "grid.h"
#include "numbers.h"
#include "order.h"
#include "parse.h"

struct Range {
    long long begin;
    long long end;
};


int main() {
    auto [top, bottom] = Split2(Split(Trim(GetContents("input.txt")), "\n"), {""});
    std::vector<Range> ranges;

    for (const std::string& line : top) {
        auto [left, right] = SplitN(line, "-");
        ranges.push_back(Range{std::stoll(left), std::stoll(right)});
    }
    auto is_fresh = [&](long long x) {
        for (const Range& range : ranges) {
            if (x >= range.begin && x <= range.end) {
                return true;
            }
        }
        return false;
    };

    int answer = 0;
    for (const std::string& line : bottom) {
        long long x = std::stoll(line);
        if (is_fresh(x)) {
            answer++;
        }
    }
    std::cout << answer << std::endl;

    return 0;
}