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
    std::string input = Trim(GetContents("input.txt"));
    for (char&c : input) {
        if (c != '-' && !isdigit(c)) {
            c = ' ';
        }
    }

    int answer = 0;
    for (int x : ParseVector<int>(input)) {
        answer += x;
    }
    std::cout << answer << std::endl;
    return 0;
}