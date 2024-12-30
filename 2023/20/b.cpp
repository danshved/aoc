#include <iostream>
#include <queue>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

#include "parse.h"
#include "numbers.h"

enum ModuleType {
    kUnknown = 0,
    kBroadcaster,
    kFlipFlop,
    kConjunction
};

struct Signal {
    std::string from;
    std::string to;
    bool value;
};

// This code doesn't solve the problem itself, but it helps. It prints out the steps
// when a given module receives non-zero low pulses. Modules &xc, &th, &pd, &bp
// do it every 3847, 4001, 3877, and 3823 steps, respectively. The answer is
// the product (and the least common multiple) of these 4 numbers.
int main() {
    std::unordered_map<std::string, std::vector<std::string>> outputs;
    std::unordered_map<std::string, std::vector<std::string>> inputs;
    std::unordered_map<std::string, ModuleType> types;
    for (const std::string& line : Split(Trim(GetContents("input.txt")), "\n")) {
        auto [l, r] = SplitN(line, " -> ");
        std::string name;
        if (l[0] == '%') {
            name = l.substr(1);
            types[name] = kFlipFlop;
        } else if (l[0] == '&') {
            name = l.substr(1);
            types[name] = kConjunction;
        } else {
            assert(l == "broadcaster");
            name = l;
            types[name] = kBroadcaster;
        }

        for (const std::string& out : ParseVector<std::string>(r)) {
            outputs[name].push_back(out);
            inputs[out].push_back(name);
        }
    }

    std::unordered_map<std::string, bool> states;
    std::unordered_map<std::string, std::unordered_map<std::string, bool>> in_states;
    for (const auto& [module, ins] : inputs) {
        ModuleType t = types[module];
        if (t == kConjunction) {
            for (const std::string& in : ins) {
                in_states[module][in] = false;
            }
        } else if (t == kFlipFlop) {
            states[module] = false;
        }
    }

    for (int step = 1; step < 100000; step++) {
        std::unordered_map<bool, int> watch_counts;

        std::queue<Signal> pulses;
        pulses.push({"button", "broadcaster", false});
        while (!pulses.empty()) {
            Signal cur = pulses.front();
            pulses.pop();
            auto send = [&](bool value) {
                for (const std::string& out : outputs.at(cur.to)) {
                    pulses.push({cur.to, out, value});
                }
            };
            if (cur.to == "bp") {
                watch_counts[cur.value]++;
            }
            switch (types[cur.to]) {
                case kUnknown:
                    break;
                case kBroadcaster:
                    send(cur.value);
                    break;
                case kFlipFlop:
                    if (!cur.value) {
                        bool& state = states.at(cur.to);
                        send(state = !state);
                    }
                    break;
                case kConjunction:
                    in_states.at(cur.to).at(cur.from) = cur.value;
                    send(std::ranges::count(in_states.at(cur.to) | std::views::values, false) != 0);
                    break;
                default:
                    assert(false);
            }
        }
        if (watch_counts[false] != 0) {
            std::cout << step << ": " << watch_counts[false] << " " << watch_counts[true] << std::endl;
        }
    }
    return 0;
}