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

int Hash(const std::string& s) {
    int x = 0;
    for (char c : s) {
        x += (int)(unsigned char)c;
        x *= 17;
        x &= 0xff;
    }
    return x;
}

int main() {
    std::vector<std::string> ss = Split(Trim(GetContents("input.txt")), ',');
    int answer = 0;
    for (const std::string& s : ss) {
        answer += Hash(s);
    }

    std::cout << answer << std::endl;
    return 0;
}