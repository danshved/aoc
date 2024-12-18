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
#include "numbers.h"
#include "order.h"
#include "parse.h"

const int kSizeX = 71;
const int kSizeY = 71;

struct Coord {
    int x, y;
};

const Coord kDirs[4] = {{1, 0}, {0, -1}, {-1, 0}, {0, 1}};
const int kInf = std::numeric_limits<int>::max();

bool InBounds(const Coord& c) {
    return c.x >= 0 && c.x < kSizeX && c.y >= 0 && c.y < kSizeY;
}

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), '\n');
    std::vector<Coord> bytes;
    for (const std::string& line : lines) {
        auto [l, r] = Split2(line, ',');
        bytes.push_back(Coord{std::stoi(l), std::stoi(r)});
        assert (InBounds(bytes.back()));
    }

    NestedVector<2, bool> occupied = ConstVector(false, kSizeY, kSizeX);
    for (int i = 0; i < 1024; i++) {
        occupied[bytes[i].y][bytes[i].x] = true;
    }

    NestedVector<2, int> d = ConstVector(kInf, kSizeY, kSizeX);
    std::queue<Coord> q;
    q.push({0, 0});
    d[0][0] = 0;
    while (!q.empty()) {
        Coord u = q.front();
        q.pop();
        for (int dir = 0; dir < 4; dir++) {
            Coord v = {u.x + kDirs[dir].x, u.y + kDirs[dir].y};
            if (!InBounds(v) || occupied[v.y][v.x]) {
                continue;
            }
            if (d[v.y][v.x] == kInf) {
                d[v.y][v.x] = d[u.y][u.x] + 1;
                q.push(v);
            }
        }
    }

    std::cout << d[kSizeY - 1][kSizeX - 1] << std::endl;
    return 0;
}