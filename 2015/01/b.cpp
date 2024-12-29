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
    int i = 0, level = 0;
    for (; i < input.size(); i++) {
        level += (input[i] == '(') ? 1 : -1;
        if (level < 0) {
            break;
        }
    }
    std::cout << i + 1 << std::endl;
    return 0;
}