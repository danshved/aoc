#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "collections.h"
#include "parse.h"

struct Machine {
    int ax;
    int ay;
    int bx;
    int by;
    int prize_x;
    int prize_y;
};

std::pair<int, int> ParseLine(const std::string& s) {
    auto [_, left, right] = SplitN(s, ": ", ", ");
    return std::make_pair(std::stoi(left.substr(2)), std::stoi(right.substr(2)));
}

Machine ParseMachine(const std::vector<std::string>& par) {
    assert(par.size() == 3);
    Machine m;
    std::tie(m.ax, m.ay) = ParseLine(par[0]);
    std::tie(m.bx, m.by) = ParseLine(par[1]);
    std::tie(m.prize_x, m.prize_y) = ParseLine(par[2]);
    return m;
}

int main() {
    NestedVector<2, std::string> pars = Split(Split(Trim(GetContents("input.txt")), "\n"), {""});

    int answer = 0;
    for (const std::vector<std::string>& par : pars) {
        Machine m = ParseMachine(par);

        bool found = false;
        for (int i = 0; i <= 100 && !found; i++) {
            for (int j = 0; j <= 100 && !found; j++) {
                if (m.prize_x == i * m.ax + j * m.bx && m.prize_y == i * m.ay + j * m.by) {
                    answer += 3 * i + j;
                    found = true;
                }
            }
        }
    }

    std::cout << answer << std::endl;
    return 0;
}