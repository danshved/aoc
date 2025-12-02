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

bool IsRepeating(long long id, int d) {
    std::string s = std::to_string(id);
    int length = s.size();
    if (length % d != 0) {
        return false;
    }

    int chunk_length = s.size() / d;
    for (int i = 0; i < length - chunk_length; i++) {
        if (s[i] != s[i + chunk_length]) {
            return false;
        }
    }

    return true;
}

bool IsInvalidId(long long id) {
    for (int d = 2; d <= 10; d++) {
        if (IsRepeating(id, d)) {
            return true;
        }
    }
    return false;
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