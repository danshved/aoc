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

const Keypad kNumbers = {
    {'7', {0, 0}},
    {'8', {0, 1}},
    {'9', {0, 2}},
    {'4', {1, 0}},
    {'5', {1, 1}},
    {'6', {1, 2}},
    {'1', {2, 0}},
    {'2', {2, 1}},
    {'3', {2, 2}},
    {'x', {3, 0}},
    {'0', {3, 1}},
    {'A', {3, 2}},
};

const Keypad kArrows = {
    {'x', {0, 0}},
    {'^', {0, 1}},
    {'A', {0, 2}},
    {'<', {1, 0}},
    {'v', {1, 1}},
    {'>', {1, 2}},
};

std::vector<std::string> GetVariants(const std::string& s, int i, const Keypad& kp, Coord pos) {
    if (i == s.size()) {
        return {""};
    }

    Coord next = kp.at(s[i]);
    std::vector<std::string> from_next = GetVariants(s, i + 1, kp, next);

    std::string i_moves = (next.i > pos.i) ? std::string(next.i - pos.i, 'v')
                                           : std::string(pos.i - next.i, '^');
    std::string j_moves = (next.j > pos.j) ? std::string(next.j - pos.j, '>')
                                           : std::string(pos.j - next.j, '<');
    Coord forbidden = kp.at('x');
    std::vector<std::string> result;

    if (Coord{pos.i, next.j} != forbidden) {
        std::string prefix = j_moves + i_moves + "A";
        for (const std::string& rest : from_next) {
            result.push_back(prefix + rest);
        }
    }

    if (next.i != pos.i && next.j != pos.j && Coord{next.i, pos.j} != forbidden) {
        std::string prefix = i_moves + j_moves + "A";
        for (const std::string& rest : from_next) {
            result.push_back(prefix + rest);
        }
    }

    return result;
}

std::vector<std::string> Get(const std::vector<std::string>& ss, const Keypad& kp) {
    return ConcatMap(ss, [&](const std::string& s) {
        return GetVariants(s, 0, kp, kp.at('A'));
    });
}

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), '\n');

    int answer = 0;
    for (const std::string& s : lines) {
        std::vector<std::string> vars = Get(Get(Get({s}, kNumbers), kArrows), kArrows);

        int best = std::numeric_limits<int>::max();
        for (const std::string& var : vars) {
            best = std::min(best, (int)var.size());
        }

        answer += std::stoi(s) * best;
    }

    std::cout << answer << std::endl;
    return 0;
}