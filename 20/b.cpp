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

struct Coord {
    int i;
    int j;

    Coord operator+(const Coord& other) const {
        return {i + other.i, j + other.j};
    }
};

const Coord kDirs[4] = {{0, 1}, {-1, 0}, {0, -1}, {1, 0}};
const int kInf = std::numeric_limits<int>::max();

int main() {
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), '\n');
    auto [size_i, size_j] = Sizes<2>(input);
    auto in_bounds = [&](const Coord& c) {
        return c.i >= 0 && c.i < size_i && c.j >= 0 && c.j < size_j;
    };

    Coord start, end;
    std::tie(start.i, start.j) = FindOrDie<2>(input, 'S');
    std::tie(end.i, end.j) = FindOrDie<2>(input, 'E');

    NestedVector<2, int> d = ConstVector(kInf, size_i, size_j);
    std::queue<Coord> q;
    d[start.i][start.j] = 0;
    q.push(start);
    while (!q.empty()) {
        Coord u = q.front();
        q.pop();
        for (int dir = 0; dir < 4; dir++) {
            Coord v = u + kDirs[dir];
            if (!in_bounds(v) || input[v.i][v.j] == '#') {
                continue;
            }
            if (d[v.i][v.j] == kInf) {
                d[v.i][v.j] = d[u.i][u.j] + 1;
                q.push(v);
            }
        }
    }

    int answer = 0;
    for (int i = 0; i < size_i; i++) {
        for (int j = 0; j < size_j; j++) {
            if (input[i][j] == '#') {
                continue;
            }
            for (int i1 = 0; i1 < size_i; i1++) {
                for (int j1 = 0; j1 < size_j; j1++) {
                    if (input[i1][j1] == '#') {
                        continue;
                    }
                    int md = abs(i - i1) + abs(j - j1);
                    if (md > 20) {
                        continue;
                    }
                    assert (d[i][j] != kInf);
                    assert (d[i1][j1] != kInf);

                    int cheat = d[i1][j1] - d[i][j] - md;
                    if (cheat >= 100) {
                        answer++;
                    }
                }
            }
        }
    }

    std::cout << answer << std::endl;
    return 0;
}