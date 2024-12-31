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

struct Pair {
    Coord sensor;
    Coord beacon;
};

Pair ParsePair(const std::string& s) {
    std::vector<int> values;
    for (const std::string& word : Split(s, " ")) {
        if (int pos = word.find('='); pos != std::string::npos) {
            values.push_back(std::stoi(word.substr(pos + 1)));
        }
    }
    assert(values.size() == 4);
    return {{values[1], values[0]}, {values[3], values[2]}};
}

const int kMax = 4000000;
Box kBox = {kMax + 1, kMax + 1};

int main() {
    std::vector<Pair> pairs;
    for (const std::string& line : Split(Trim(GetContents("input.txt")), "\n")) {
        pairs.push_back(ParsePair(line));
    }

    auto is_far = [&](const Coord& pos) {
        return std::ranges::all_of(pairs, [&](const Pair& p) {
            return (pos - p.sensor).Manhattan() > (p.beacon - p.sensor).Manhattan();
        });
    };

    for (const Pair& p : pairs) {
        int radius = (p.beacon - p.sensor).Manhattan();
        for (const Coord& c : ManhattanCircle(p.sensor, radius + 1)) {
            if (kBox.contains(c) && is_far(c)) {
                std::cout << c.i + c.j * 4000000ll << std::endl;
            }
        }
    }

    return 0;
}