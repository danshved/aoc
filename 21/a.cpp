#include <iostream>
#include <limits>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

#include "collections.h"
#include "grid.h"
#include "parse.h"

using Keypad = std::unordered_map<char, Coord>;

// clang-format off
const Keypad kDigits = {
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

std::vector<std::string> GetVariants(const std::string& s, int i, const Keypad& kp, Coord pos) {
    if (i == s.size()) {
        return {""};
    }

    Coord next = kp.at(s[i]);
    std::vector<std::string> from_next = GetVariants(s, i + 1, kp, next);
    std::string i_moves = (next.i > pos.i) ? std::string(next.i - pos.i, 'v') : std::string(pos.i - next.i, '^');
    std::string j_moves = (next.j > pos.j) ? std::string(next.j - pos.j, '>') : std::string(pos.j - next.j, '<');

    std::vector<std::string> result;
    if (Coord{pos.i, next.j} != kp.at(' ')) {
        std::string prefix = j_moves + i_moves + "A";
        for (const std::string& rest : from_next) {
            result.push_back(prefix + rest);
        }
    }
    if (next.i != pos.i && next.j != pos.j && Coord{next.i, pos.j} != kp.at(' ')) {
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
    int answer = 0;
    for (const std::string& s : Split(Trim(GetContents("input.txt")), "\n")) {
        std::vector<std::string> vars = Get(Get(Get({s}, kDigits), kArrows), kArrows);
        int best = std::ranges::min(vars | std::views::transform([&](const std::string& var) {
                                        return (int)var.size();
                                    }));
        answer += std::stoi(s) * best;
    }

    std::cout << answer << std::endl;
    return 0;
}