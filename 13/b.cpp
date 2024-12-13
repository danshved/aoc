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

#include "order.h"
#include "numbers.h"
#include "parse.h"

struct Machine {
    long long ax;
    long long ay;
    long long bx;
    long long by;
    long long prize_x;
    long long prize_y;
};

std::pair<long long, long long> ParseLine(const std::string& s) {
    auto [unused, words] = Split2(s, ':');
    auto [left, right] = Split2(Trim(words), ' ');
    return std::make_pair(std::stoll(left.substr(2)), std::stoll(right.substr(2)));
}

Machine ParseMachine(const std::vector<std::string>& par) {
    assert (par.size() == 3);
    Machine m;
    std::tie(m.ax, m.ay) = ParseLine(par[0]);
    std::tie(m.bx, m.by) = ParseLine(par[1]);
    std::tie(m.prize_x, m.prize_y) = ParseLine(par[2]);
    m.prize_x += 10000000000000LL;
    m.prize_y += 10000000000000LL;
    return m;
}

template<typename T>
T Det(T a, T b, T c, T d) {
    return a * d - b * c;
}

int main() {
    std::vector<std::vector<std::string>> pars = Split(Split(Trim(GetContents("input.txt")), '\n'), std::string());

    long long answer = 0;
    for (const std::vector<std::string>& par : pars) {
        Machine m = ParseMachine(par);

        long long d = Det(m.ax, m.ay, m.bx, m.by);
        long long i = Det(m.prize_x, m.prize_y, m.bx, m.by);
        long long j = Det(m.ax, m.ay, m.prize_x, m.prize_y);

        assert (d != 0);
        if (i % d != 0 || j % d != 0) {
            continue;
        }
        if (i / d < 0 || j / d < 0) {
            continue;
        }
        answer += 3 * (i / d) + (j / d);
    }

    std::cout << answer << std::endl;
    return 0;
}