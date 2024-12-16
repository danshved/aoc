#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
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

    Coord operator+(const Coord& other) {
        return {i + other.i, j + other.j};
    }
};

struct Update {
    int dist;
    Coord pos;
    int dir;

    std::strong_ordering operator<=>(const Update& other) const {
        return other.dist <=> dist;
    }
};

const Coord kDirs[4] = {{0, 1}, {-1, 0}, {0, -1}, {1, 0}};
enum { kEast, kNorth, kWest, kSouth };
const int kInf = std::numeric_limits<int>::max();

NestedVector<3, int> DistancesFrom(const std::vector<std::string>& input,
                       Coord start, int start_dir) {
    auto [size_i, size_j] = Sizes<2>(input);
    auto in_bounds = [&](const Coord& c) {
        return c.i >= 0 && c.i < size_i && c.j >= 0 && c.j < size_j;
    };

    NestedVector<3, int> d = ConstVector(kInf, size_i, size_j, 4);
    std::vector<Update> q;
    auto push = [&](const Update &u) {
        q.push_back(u);
        std::push_heap(q.begin(), q.end());
    };
    auto pop = [&]() {
        std::pop_heap(q.begin(), q.end());
        Update u = q.back();
        q.pop_back();
        return u;
    };

    push(Update{0, start, start_dir});
    while (!q.empty()) {
        Update u = pop();
        if (d[u.pos.i][u.pos.j][u.dir] < u.dist) {
            continue;
        }
        d[u.pos.i][u.pos.j][u.dir] = u.dist;

        push(Update{u.dist + 1000, u.pos, (u.dir + 1) % 4});
        push(Update{u.dist + 1000, u.pos, (u.dir + 3) % 4});
        Coord new_pos = u.pos + kDirs[u.dir];
        if (in_bounds(new_pos) && input[new_pos.i][new_pos.j] != '#') {
            push(Update{u.dist + 1, new_pos, u.dir});
        }
    }

    return d;
}

int main() {
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), '\n');
    auto [size_i, size_j] = Sizes<2>(input);

    Coord start, end;
    std::tie(start.i, start.j) = FindOrDie<2>(input, 'S');
    std::tie(end.i, end.j) = FindOrDie<2>(input, 'E');

    NestedVector<3, int> from_start = DistancesFrom(input, start, kEast);
    NestedVector<3, int> from_end = DistancesFrom(input, end, kSouth);

    // Find all states lying on a shortest path. State s lies on a shortest
    // path from b to e if and only if dist(b, e) = dist(b, s) + dist(s, e).
    int answer = 0;
    for (int i = 0; i < size_i; i++) {
        for (int j = 0; j < size_j; j++) {
            for (int dir = 0; dir < 4; dir++) {
                if (from_start[i][j][dir] == kInf || from_end[i][j][(dir + 2) % 4] == kInf) {
                    continue;
                }
                if (from_start[i][j][dir] + from_end[i][j][(dir + 2) % 4] ==
                    from_start[end.i][end.j][kNorth]) {
                    answer++;
                    break;
                }
            }
        }
    }

    std::cout << "Part 1: " << from_start[end.i][end.j][kNorth] << std::endl;
    std::cout << "Part 2: " << answer << std::endl;
    return 0;
}