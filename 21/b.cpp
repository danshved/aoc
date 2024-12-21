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

    bool operator==(const Coord&) const = default;
};

using Keypad = std::unordered_map<char, Coord>;

// clang-format off
const Keypad kNumbers = {
    {'7', {0, 0}}, {'8', {0, 1}}, {'9', {0, 2}},
    {'4', {1, 0}}, {'5', {1, 1}}, {'6', {1, 2}},
    {'1', {2, 0}}, {'2', {2, 1}}, {'3', {2, 2}},
    {' ', {3, 0}}, {'0', {3, 1}}, {'A', {3, 2}},
};
const Keypad kArrows = {
    {' ', {0, 0}}, {'^', {0, 1}}, {'A', {0, 2}},
    {'<', {1, 0}}, {'v', {1, 1}}, {'>', {1, 2}},
};
// clang-format on

long long MoveCost(Coord, Coord, const Keypad&, int);

// How many human keypresses it costs to push a given sequence of keys on a
// given keypad, assuming that the pointer on this keypad and all the pointers
// on keypads upstream start on 'A' and finish on 'A'.
long long StrCost(const std::string& s, const Keypad& kp, int robots) {
    Coord pos = kp.at('A');
    long long result = 0;
    for (char c : s) {
        Coord next = kp.at(c);
        result += MoveCost(pos, next, kp, robots) + 1;
        pos = next;
    }
    result += MoveCost(pos, kp.at('A'), kp, robots);
    return result;
}

// How many human keypresses it costs to move the pointer on a given keypad
// from `start` to `end`, assuming all pointers upstream start on 'A' and finish
// on 'A'.
long long MoveCost(Coord start, Coord end, const Keypad& kp, int robots) {
    static NestedVector<5, long long> d = ConstVector(-1ll, 4, 3, 4, 3, 27);
    long long& result = d[start.i][start.j][end.i][end.j][robots];
    if (result != -1) {
        return result;
    }

    if (robots == 0) {
        // The pointer is a human finger, it costs nothing to move it.
        return result = 0;
    }

    // The pointer is a robotic arm. Try to move it in two ways:
    // horizontal-then-vertical and vertical-then-horizontal.
    std::string i_moves = (end.i > start.i) ? std::string(end.i - start.i, 'v') : std::string(start.i - end.i, '^');
    std::string j_moves = (end.j > start.j) ? std::string(end.j - start.j, '>') : std::string(start.j - end.j, '<');

    result = std::numeric_limits<long long>::max();
    if (Coord{start.i, end.j} != kp.at(' ')) {
        result = std::min(result, StrCost(j_moves + i_moves, kArrows, robots - 1));
    }
    if (Coord{end.i, start.j} != kp.at(' ')) {
        result = std::min(result, StrCost(i_moves + j_moves, kArrows, robots - 1));
    }
    return result;
}

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), '\n');
    long long answer = 0;
    for (const std::string& s : lines) {
        answer += std::stoll(s) * StrCost(s, kNumbers, 26);
    }
    std::cout << answer << std::endl;
    return 0;
}