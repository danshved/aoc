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

struct State {
    int a, b, c;
};

int ParseRegister(const std::string& s) {
    return std::stoi(Split(s, ' ')[2]);
}

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), '\n');

    State s = {
        .a = ParseRegister(lines[0]),
        .b = ParseRegister(lines[1]),
        .c = ParseRegister(lines[2]),
    };
    std::vector<int> program = ParseVector<int>(Split2(lines[4], ' ').second);
    std::vector<int> output;

    int pc = 0;
    auto combo = [&]() {
        switch(program[pc + 1]) {
            case 0:
            case 1:
            case 2:
            case 3: return program[pc + 1];
            case 4: return s.a;
            case 5: return s.b;
            case 6: return s.c;
            default: assert(false);
        }
    };
    while (pc < program.size()) {
        assert (pc != program.size() - 1);
        
        switch(program[pc]) {
            case 0: s.a >>= combo(); pc += 2; break;
            case 1: s.b ^= program[pc + 1]; pc += 2; break;
            case 2: s.b = combo() & 07; pc += 2; break;
            case 3: if (s.a != 0) {pc = program[pc + 1];}
                    else {pc += 2;} break;
            case 4: s.b ^= s.c; pc += 2; break;
            case 5: output.push_back(combo() & 07); pc += 2; break;
            case 6: s.b = s.a >> combo(); pc += 2; break;
            case 7: s.c = s.a >> combo(); pc += 2; break;
        }
    }

    std::cout << FormatVector(output, ",");
    return 0;
}