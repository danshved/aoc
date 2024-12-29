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
#include <cassert>

#include "order.h"
#include "numbers.h"
#include "parse.h"

std::string seq;
std::unordered_map<std::string, std::pair<std::string, std::string>> pairs;

struct State {
    std::string name;
    int pos;

    bool operator==(const State&) const = default;
};

State Next(State s) {
    char c = seq[s.pos];
    return State{(c == 'L') ? pairs[s.name].first : pairs[s.name].second,
        (s.pos + 1) % (int)seq.size()};
}

bool IsFinal(State s) {
    return s.name[2] == 'Z';
}

template<typename T>
class StateMap {
public:
    void Init(const T& value) {
        std::unordered_map<std::string, T> m;
        for (const auto& [k, v] : pairs) {
            m[k] = value;
        }
        data_.assign(seq.size(), m);
    }

    T& operator[](const State& s) {
        return data_[s.pos][s.name];
    }

private:
    std::vector<std::unordered_map<std::string, T>> data_;
};

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), '\n');
    std::vector<std::vector<std::string>> pars = Split(lines, std::string());
    assert(pars.size() == 2);
    assert(pars[0].size() == 1);

    seq = pars[0][0];
    std::vector<State> starts;
    for (const std::string& s : pars[1]) {
        std::string k = s.substr(0, 3);
        if (k[2] == 'A') {
            starts.push_back(State{k, 0});
        }
        pairs[k] = std::make_pair(s.substr(7,3), s.substr(12, 3));
        assert(s.length() == 16);
    }

    std::vector<long long> periods;
    long long answer = 1;
    for (State s : starts) {
        StateMap<int> number;
        number.Init(-1);
        int t = 0;
        while (number[s] == -1) {
            number[s] = t++;
            s = Next(s);
        }
        int period = t - number[s];

        answer *= (period / (int)seq.size());
    }
    answer *= (int)seq.size();
    std::cout << answer << std::endl;

    // Well, there's no reason this solution would work in general, but the
    // input is clearly constructed artificially to make it easy. All periods
    // have gcd == seq.size() with each other, and each period contains precisely
    // one final state. To make things even simpler, that happens exactly at
    // times that are == 0 (mod period), so we don't need to bother with the
    // Chinese Remainder Theorem.

    return 0;
}