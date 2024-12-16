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

const Coord kDirs[4] = {{0, 1}, {-1, 0}, {0, -1}, {1, 0}};
const int kInf = std::numeric_limits<int>::max();

template <typename T>
using VVV = std::vector<std::vector<std::vector<T>>>;

template <typename T>
VVV<T> MakeVVV(int size1, int size2, int size3, const T& val) {
    return VVV<T>(size1, std::vector<std::vector<T>>(
                             size2, std::vector<T>(size3, val)));
}

int main() {
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), '\n');
    int size_i = input.size();
    int size_j = input[0].size();

    Coord start, end;
    std::tie(start.i, start.j) = FindOrDie<2>(input, 'S');
    std::tie(end.i, end.j) = FindOrDie<2>(input, 'E');

    auto in_bounds = [&](const Coord& c) {
        return c.i >= 0 && c.i < size_i && c.j >= 0 && c.j < size_j;
    };

    VVV<int> d = MakeVVV(size_i, size_j, 4, kInf);
    VVV<bool> added = MakeVVV(size_i, size_j, 4, false);
    d[start.i][start.j][0] = 0;

    while (true) {
        // Find a node with the lowest distance that hasn't been finalized yet.
        Coord cur;
        int cur_dir;
        int dist = kInf;
        for (int i = 0; i < size_i; i++) {
            for (int j = 0; j < size_j; j++) {
                for (int dir = 0; dir < 4; dir++) {
                    if (added[i][j][dir]) {
                        continue;
                    }
                    if (d[i][j][dir] < dist) {
                        dist = d[i][j][dir];
                        std::tie(cur.i, cur.j, cur_dir) = std::make_tuple(i, j, dir);
                    }
                }
            }
        }
        if (dist == kInf) {
            break;
        }

        // Finalize it.
        added[cur.i][cur.j][cur_dir] = true;

        // Update distances to its 2 or 3 outgoing neighbors.
        for (int delta = 1; delta <= 3; delta += 2) {
            int new_dir = (cur_dir + delta) % 4;
            int new_dist = dist + 1000;
            if (new_dist < d[cur.i][cur.j][new_dir]) {
                d[cur.i][cur.j][new_dir] = new_dist;
            }
        }
        {
            Coord new_c = cur + kDirs[cur_dir];
            int new_dist = dist + 1;
            if (in_bounds(new_c) && input[new_c.i][new_c.j] != '#' &&
                new_dist < d[new_c.i][new_c.j][cur_dir]) {
                d[new_c.i][new_c.j][cur_dir] = new_dist;
            }
        }
    }

    int answer = kInf;
    for (int dir = 0; dir < 4; dir++) {
        answer = std::min(answer, d[end.i][end.j][dir]);
    }

    std::cout << answer << std::endl;
    return 0;
}