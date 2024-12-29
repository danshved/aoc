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

bool HasDouble(const std::string& s) {
    for (int i = 0; i + 2 < s.size(); i++) {
        if (s[i] == s[i + 2]) {
            return true;
        }
    }
    return false;
}

bool IsNice(const std::string& s) {
    if (!HasDouble(s)) {
        return false;
    }
    for (int i = 0; i + 2 < s.size(); i++) {
        if (Split(s, s.substr(i, 2)).size() >= 3) {
            return true;
        }
    }
    return false;
}

int main() {
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), "\n");
    std::cout << std::ranges::count_if(input, IsNice) << std::endl;
    return 0;
}