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

struct State {
    std::string valve;
    int rate;
    std::unordered_set<std::string> open;
    int time_left;

    bool operator==(const State&) const = default;

    State DoNothing() const {
        State s = *this;
        s.time_left--;
        return s;
    }

    State OpenValve(std::string v, int v_rate) const {
        State s = *this;
        s.open.insert(v);
        s.rate += v_rate;
        s.time_left--;
        return s;
    }

    State MoveTo(std::string v) const {
        State s = *this;
        s.valve = v;
        s.time_left--;
        return s;
    }
};

template <>
struct std::hash<State> {
    size_t operator()(const State& s) const {
        size_t x = 0;
        std::vector<std::string> v(s.open.begin(), s.open.end());
        std::ranges::sort(v);
        for (const std::string& s : v) {
            x = CombineHash(x, std::hash<std::string>()(s));
        }
        return CombineHash(x, SeqHash(s.valve, s.rate, s.time_left));
    }
};

int main() {
    std::unordered_map<std::string, int> rates;
    std::unordered_map<std::string, std::vector<std::string>> edges;

    for (const std::string& line : Split(Trim(GetContents("input.txt")), '\n')) {
        // Valve EG has flow rate=21; tunnels lead to valves WZ, OF, ZP, QD
        std::vector<std::string> words = Split(line, ' ');
        std::string valve = words[1];
        rates[valve] = std::stoi(words[4].substr(words[4].find('=') + 1));
        for (int i = 9; i < words.size(); i++) {
            edges[valve].push_back(words[i].substr(0, words[i].find(',')));
        }
    }

    std::unordered_map<State, int> mem;
    auto best = [&](auto& self, const State& s) {
        if (mem.contains(s)) {
            return mem[s];
        }
        if (s.time_left == 0) {
            return mem[s] = 0;
        }

        int total = self(self, s.DoNothing());
        for (const std::string& v : edges[s.valve]) {
            total = std::max(total, self(self, s.MoveTo(v)));
        }
        if (!s.open.contains(s.valve) && rates[s.valve] > 0) {
            total = std::max(total, self(self, s.OpenValve(s.valve, rates[s.valve])));
        }
        return mem[s] = total + s.rate;
    };

    std::cout << best(best, {"AA", 0, {}, 30}) << " " << mem.size() << std::endl;
    return 0;
}