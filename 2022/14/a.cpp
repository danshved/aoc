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

int Clamp(int i) {
    return std::max(-1, std::min(1, i));
}

Coord Clamp(Coord c) {
    return {Clamp(c.i),Clamp(c.j)};
}

const Coord kStart = {0, 500};
const Coord kMoves[3] = {kSouth, kSouth + kWest, kSouth + kEast};

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), '\n');

    std::unordered_map<Coord, char> blocks;
    int max_i = 0;
    for (const std::string& s : lines) {
        std::vector<std::string> ss = Split(s, ' ');
        std::vector<Coord> vertices;
        for (int i = 0; 2 * i < ss.size(); i++) {
            auto [l, r] = Split2(ss[2 * i], ',');
            vertices.emplace_back(std::stoi(r), std::stoi(l));
        }
        assert (vertices.size() >= 2);

        for (int i = 0; i + 1 < vertices.size(); i++) {
            Coord from = vertices[i], to = vertices[i + 1];
            Coord dir = Clamp(to - from);
            for(; from != to; from += dir) {
                blocks[from] = '#';
            }
            blocks[from] = '#';
            max_i = std::max(max_i, std::max(from.i, to.i));
        }
    }

    int answer = 0;
    while(true) {
        Coord sand = kStart;
        while (sand.i <= max_i) {
            auto it = std::ranges::find_if(kMoves, [&](Coord move) {
                return !blocks.contains(sand + move);
            });
            if (it == std::end(kMoves)) {
                break;
            }
            sand += *it;
        }
        if (sand.i <= max_i) {
            blocks[sand] = 'o';
            answer++;
        } else {
            break;
        }
    }

    std::cout << answer << std::endl;
    return 0;
}