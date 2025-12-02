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

bool IsInvalidId(long long id) {
    std::string s = std::to_string(id);
    if (s.size() % 2 != 0) {
        return false;
    }

    int half = s.size() / 2;
    for (int i = 0; i < half; i++) {
        if (s[i] != s[i + half]) {
            return false;
        }
    }

    return true;
}

int main() {
    long long answer = 0;
    for (const std::string& range : Split(Trim(GetContents("input.txt")), ",")) {
        auto [left, right] = SplitN(range, "-");
        long long from = std::stoll(left);
        long long to = std::stoll(right);
        for (long long i = from; i <= to; i++) {
            if (IsInvalidId(i)) {
                answer += i;
            }
        }
    }
    std::cout << answer << std::endl;
    return 0;
}