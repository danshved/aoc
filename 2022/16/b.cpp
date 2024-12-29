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
#include "graph_search.h"
#include "grid.h"
#include "numbers.h"
#include "order.h"
#include "parse.h"

// The solution visits 241'733'645 of these states. There was disk I/O.
// Even freeing the memory after printing the answer took several minutes.
struct State {
    std::string player;
    std::string elephant;
    int rate;
    int mask;
    int time_left;

    bool operator==(const State&) const = default;

    void OpenValve(int index, int v_rate) {
        if (v_rate > 0 && (mask & (1 << index)) == 0) {
            mask |= 1 << index;
            rate += v_rate;
        }
    }

    void Normalize() {
        if (player > elephant) {
            std::swap(player, elephant);
        }
    }
};

template <>
struct std::hash<State> {
    size_t operator()(const State& s) const {
        return SeqHash(s.player, s.elephant, s.rate, s.mask, s.time_left);
    }
};

int main() {
    std::unordered_map<std::string, int> rates;
    std::unordered_map<std::string, std::vector<std::string>> moves;
    std::unordered_map<std::string, int> valve_indices;

    for (const std::string& line : Split(Trim(GetContents("input.txt")), '\n')) {
        // Valve EG has flow rate=21; tunnels lead to valves WZ, OF, ZP, QD
        std::vector<std::string> words = Split(line, ' ');
        std::string valve = words[1];
        rates[valve] = std::stoi(words[4].substr(words[4].find('=') + 1));
        for (int i = 9; i < words.size(); i++) {
            moves[valve].push_back(words[i].substr(0, words[i].find(',')));
        }
        moves[valve].push_back("open");

        if (rates[valve] > 0) {
            int index = valve_indices.size();
            valve_indices[valve] = index;
            std::cout << valve << " -> " << index << std::endl;
        }
    }

    std::unordered_map<State, int> mem;
    auto best = [&](auto& self, const State& s) {
        if (mem.contains(s)) {
            return mem[s];
        }
        if (s.time_left == 0) {
            if (mem.size() % 1000000 == 0) {
                std::cout << mem.size() << std::endl;
            }
            return mem[s] = 0;
        }

        int total = std::numeric_limits<int>::min();
        for (const std::string& move : moves[s.player]) {
            for (const std::string& el_move : moves[s.elephant]) {
                State t = s;
                t.time_left--;

                if (move == "open") {
                    if (valve_indices.contains(t.player)) {
                        t.OpenValve(valve_indices[t.player], rates[t.player]);
                    } else {
                        assert((rates[t.player] == 0));
                    }
                } else {
                    t.player = move;
                }

                if (el_move == "open") {
                    if (valve_indices.contains(t.elephant)) {
                        t.OpenValve(valve_indices[t.elephant], rates[t.elephant]);
                    }
                } else {
                    t.elephant = el_move;
                }

                t.Normalize();
                total = std::max(total, self(self, t));
            }
        }

        if (mem.size() % 1000000 == 0) {
            std::cout << mem.size() << std::endl;
        }
        return mem[s] = total + s.rate;
    };

    std::cout << best(best, {"AA", "AA", 0, 0, 26}) << std::endl;
    std::cout << mem.size() << std::endl;
    return 0;
}