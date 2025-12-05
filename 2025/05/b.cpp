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

    bool operator==(const Range& other) const = default;
};

template<>
struct std::hash<Range> {
    size_t operator()(const Range& x) const {
        return SeqHash(x.begin, x.end);
    }
};


int main() {
    auto [top, bottom] = Split2(Split(Trim(GetContents("input.txt")), "\n"), {""});
    std::vector<Range> ranges;

    for (const std::string& line : top) {
        auto [left, right] = SplitN(line, "-");
        ranges.push_back(Range{std::stoll(left), std::stoll(right)});
    }

    Range joined;
    long long answer = 0;
    DFS<Range>(
        [&](auto& search) {
            for (const Range& u : ranges) {
                joined.begin = std::numeric_limits<long long>::max();
                joined.end = std::numeric_limits<long long>::min();
                if (search.Look(u) == DFSEdge::kTree) {
                    answer += joined.end - joined.begin + 1;
                }
            }
        },
        [&](auto& search, Range u) {
            joined.begin = std::min(joined.begin, u.begin);
            joined.end = std::max(joined.end, u.end);
            for (const Range& v : ranges) {
                if (v.begin > u.end || u.begin > v.end) {
                    continue;
                }
                search.Look(v);
            }
        }
    );
    std::cout << answer << std::endl;
   
    return 0;
}