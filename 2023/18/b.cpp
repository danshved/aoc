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

const Coord kDirArray[4] = {kEast, kSouth, kWest, kNorth};

int FromHex(const std::string& s) {
    int result = 0;
    std::istringstream(s) >> std::hex >> result;
    return result;
}

int main() {
    Coord pos = {0, 0};
    long long double_area = 0;
    long long border = 0;

    for (const std::string& line : Split(Trim(GetContents("input.txt")), "\n")) {
        std::string s = Split(line, " ")[2];
        Coord dir = kDirArray[s[7] - '0'];
        int steps = FromHex(s.substr(2, 5));

        pos += dir * steps;
        double_area += (1ll * pos.i * dir.j - 1ll * pos.j * dir.i) * steps;
        border += steps;
    }

    assert((pos == Coord{0, 0}));
    std::cout << ((abs(double_area) + border) / 2 + 1) << std::endl;
    return 0;
}