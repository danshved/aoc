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

constexpr int kN = 12;

int main() {
    long long answer = 0;
    for (const std::string& line : Split(Trim(GetContents("input.txt")), "\n")) {
        NestedVector<2, long long> dp = ConstVector(0ll, kN + 1, line.size() + 1);
        for (int length = 1; length <= kN; length++) {
            for (int i = 0; i <= line.size(); i++) {
                dp[length][i] = -1;
                if (i > 0 && dp[length][i-1] >= 0) {
                    dp[length][i] = dp[length][i-1];
                }
                if (i > 0 && dp[length-1][i-1] >= 0) {
                    dp[length][i] = std::max(dp[length][i], 10 * dp[length-1][i-1] + (static_cast<long long>((line[i-1] - '0'))));
                }
            }
        }
        answer += dp[kN][line.size()];        
    }
    std::cout << answer << std::endl;

    return 0;
}