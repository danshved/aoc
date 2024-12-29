#include <iostream>
#include <vector>
#include <string>

#include "parse.h"

struct Case {
    long long result;
    std::vector<long long> xs;
};

int main() {
    std::vector<Case> cases;
    for (const std::string& line : Split(Trim(GetContents("input.txt")), "\n")) {
        auto [l, r] = SplitN(line, ": ");
        cases.push_back(Case{std::stoll(l), ParseVector<long long>(r)});
    }

    long long answer = 0;
    for (const Case& c : cases) {
        std::vector<long long> rs(1, c.result);

        for (int i = c.xs.size() - 1; i > 0; i--) {
            long long x = c.xs[i];
            std::vector<long long> new_rs;
            for (long long r : rs) {
                if (r >= x) {
                    new_rs.push_back(r - x);
                }
                if (r % x == 0) {
                    new_rs.push_back(r / x);
                }
            }
            rs = std::move(new_rs);
        }

        if (std::find(rs.begin(), rs.end(), c.xs[0]) != rs.end()) {
            answer += c.result;
        }
    }

    std::cout << answer << std::endl;
    return 0;
}