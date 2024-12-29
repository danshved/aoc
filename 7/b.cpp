#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "parse.h"

struct Case {
    long long result;
    std::vector<long long> xs;
};

std::optional<long long> Uncat(long long x, long long y) {
    std::string a = std::to_string(x);
    std::string b = std::to_string(y);
    if (a.size() <= b.size() || !std::equal(b.rbegin(), b.rend(), a.rbegin())) {
        return std::nullopt;
    }
    return std::stoll(a.substr(0, a.size() - b.size()));
}

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

                std::optional<long long> k = Uncat(r, x);
                if (k.has_value()) {
                    new_rs.push_back(*k);
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