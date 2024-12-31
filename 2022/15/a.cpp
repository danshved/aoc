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

const int kI = 2000000;

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

int main() {
    std::unordered_map<Coord, char> map;
    std::vector<Pair> pairs;
    for (const std::string& line : Split(Trim(GetContents("input.txt")), "\n")) {
        Pair pair = ParsePair(line);
        pairs.push_back(pair);
        map[pair.sensor] = 'S';
        map[pair.beacon] = 'B';
    }

    int answer = 0;
    for (const Pair& pair : pairs) {
        int radius = (pair.beacon - pair.sensor).Manhattan();
        int left = radius - abs(pair.sensor.i - kI);
        if (left < 0) {
            continue;
        }

        for (Coord pos : PathCC({kI, pair.sensor.j - left}, {kI, pair.sensor.j + left})) {
            if (!map.contains(pos)) {
                answer++;
                map[pos] = '#';
            }
        }
    }

    std::cout << answer << std::endl;
    return 0;
}