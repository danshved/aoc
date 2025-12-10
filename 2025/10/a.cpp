#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <optional>
#include <ranges>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "collections.h"
#include "graph_search.h"
#include "grid.h"
#include "numbers.h"
#include "order.h"
#include "parse.h"

using Mask = unsigned long long;

struct Machine {
    int lights = 0;
    Mask goal = 0;
    std::vector<Mask> buttons;
};

int GetWeight(unsigned int x) {
    int result = 0;
    while (x != 0) {
        if (x&1) {
            result++;
        }
        x >>= 1;
    }
    return result;
}

int main() {
    std::vector<Machine> machines;
    for(const std::string& line : Split(Trim(GetContents("input.txt")), "\n")) {
        Machine machine;
        for (const std::string& word : Split(line, " ")) {
            assert (word.size() >= 2);
            std::string inside = word.substr(1, word.size() - 2);
            switch(word[0]) {
                case '[': {
                    machine.lights = inside.size();
                    for (int i = 0; i < machine.lights; i++) {
                        if (inside[i] == '#') {
                            machine.goal |= Mask{1} << i;
                        }
                    }
                    break;
                }
                case '(': {
                    Mask button = 0;
                    for (const std::string& s : Split(inside, ",")) {
                        button |= Mask{1} << std::stoi(s);
                    }
                    machine.buttons.push_back(button);
                    break;
                }
                case '{': {
                    break;
                }
                default: {
                    assert(false);
                    break;
                }
            }
        }
        machines.push_back(machine);
        std::cout << machine.lights << " " << machine.goal << " " << FormatVector(machine.buttons, ",") << std::endl;
    }

    int answer = 0;
    for (const Machine& machine : machines) {
        int best = std::numeric_limits<int>::max();
        unsigned int end_pressed = (unsigned int){1} << machine.buttons.size();
        for (unsigned int pressed = 0; pressed < end_pressed; pressed++) {
            int weight = GetWeight(pressed);
            if (weight >= best) {
                continue;
            }
            Mask on = 0;
            for (int i = 0; i < machine.buttons.size(); i++) {
                if (((pressed >> i) & 1) != 0) {
                    on ^= machine.buttons[i];
                }
            }
            if (on == machine.goal) {
                best = weight;
            }
        }
        assert ((best < std::numeric_limits<int>::max()));
        answer += best;
    }
    std::cout << answer << std::endl;

    return 0;
}