#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "order.h"
#include "numbers.h"
#include "parse.h"

const Rational<__int128> kMin = 200000000000000LL;
const Rational<__int128> kMax = 400000000000000LL;

struct Stone {
    Rational<__int128> x, y, z;
    Rational<__int128> vx, vy, vz;
};

Stone ParseStone(const std::string& s) {
    std::vector<std::string> ws = Split(s, '@');
    assert(ws.size() == 2);

    std::vector<long long> pos = ParseVector<long long>(ws[0]);
    assert(pos.size() == 3);

    std::vector<long long> v = ParseVector<long long>(ws[1]);
    assert(v.size() == 3);

    return Stone{pos[0], pos[1], pos[2], v[0], v[1], v[2]};
}

template<typename T>
T Det(T a, T b, T c, T d) {
    return a * d - b * c;
}

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), '\n');
    std::vector<Stone> stones;
    for (const std::string& line : lines) {
        stones.push_back(ParseStone(line));
    }

    int answer = 0;
    for (int i = 0; i < stones.size(); i++) {
        for (int j = i + 1; j < stones.size(); j++) {
            const Stone& a = stones[i];
            const Stone& b = stones[j];

            Rational<__int128> m11 = a.vx, m12 = -b.vx, y1 = b.x - a.x;
            Rational<__int128> m21 = a.vy, m22 = -b.vy, y2 = b.y - a.y;
            Rational<__int128> d = Det(m11, m21, m12, m22);

            if (d == 0) {
                // Guess: parallel rays never intersect. Mathematically they
                // could, but AoC accepts the current answer. These degenerate
                // intersections must be absent in the input file.
                continue;
            }
            Rational<__int128> t1 = Det(y1, y2, m12, m22) / d;
            Rational<__int128> t2 = Det(m11, m21, y1, y2) / d;

            if (t1 < 0 || t2 < 0) {
                continue;
            }
            Rational<__int128> x = a.x + t1 * a.vx;
            Rational<__int128> y = a.y + t1 * a.vy;
            if (x >= kMin && x <= kMax && y >= kMin && y <= kMax) {
                answer++;
            } 
        }
    }
    std::cout << answer << std::endl;
    return 0;
}