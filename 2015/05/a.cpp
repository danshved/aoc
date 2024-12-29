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

bool IsVowel(char c) {
    return c == 'a' || c == 'o' || c == 'e' || c == 'u' || c == 'i';
}

bool HasDouble(const std::string& s) {
    for (int i = 0; i + 1 < s.size(); i++) {
        if (s[i] == s[i + 1]) {
            return true;
        }
    }
    return false;
}

bool IsNice(const std::string& s) {
    if (std::ranges::count_if(s, IsVowel) < 3) {
        return false;
    }
    if (!HasDouble(s)) {
        return false;
    }
    for (std::string bad : {"ab", "cd", "pq", "xy"}) {
        if (std::search(s.begin(), s.end(), bad.begin(), bad.end()) != s.end()) {
            return false;
        }
    }
    return true;
}

int main() {
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), "\n");
    std::cout << std::ranges::count_if(input, IsNice) << std::endl;
    return 0;
}