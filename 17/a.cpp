#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include "parse.h"

int ParseRegister(const std::string& s) {
    return std::stoi(std::get<1>(SplitN(s, ": ")));
}

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), "\n");
    int a = ParseRegister(lines[0]);
    int b = ParseRegister(lines[1]);
    int c = ParseRegister(lines[2]);
    std::vector<int> program = ParseVector<int>(std::get<1>(SplitN(lines[4], ": ")));

    std::vector<int> output;
    int pc = 0;
    auto combo = [&]() {
        // clang-format off
        switch (program[pc + 1]) {
            case 0:
            case 1:
            case 2:
            case 3: return program[pc + 1];
            case 4: return a;
            case 5: return b;
            case 6: return c;
            default: assert(false);
        }
        // clang-format on
    };
    while (pc < program.size()) {
        assert(pc != program.size() - 1);

        // clang-format off
        switch (program[pc]) {
            case 0: a >>= combo(); pc += 2; break;
            case 1: b ^= program[pc + 1]; pc += 2; break;
            case 2: b = combo() & 07; pc += 2; break;
            case 3: if (a != 0) { pc = program[pc + 1]; }
                    else { pc += 2; }
                break;
            case 4: b ^= c; pc += 2; break;
            case 5: output.push_back(combo() & 07); pc += 2; break;
            case 6: b = a >> combo(); pc += 2; break;
            case 7: c = a >> combo(); pc += 2; break;
        }
        // clang-format on
    }

    std::cout << FormatVector(output, ",");
    return 0;
}