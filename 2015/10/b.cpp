#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <queue>
#include <ranges>
#include <set>
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

int main() {
    std::string s = "3113322113";
    for (int step = 0; step < 50; step++) {
        std::ostringstream oss;
        auto it = s.begin(), end = s.end();
        while (it != end) {
            auto next = std::find_if(it, end, [&](char c) { return c != *it; });
            oss << std::ranges::distance(it, next);
            oss << *it;
            it = next;
        }
        s = oss.str();
    }
    std::cout << s.size() << std::endl;
    return 0;
}