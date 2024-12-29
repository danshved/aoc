#include <iostream>
#include <vector>

#include "parse.h"

std::unordered_map<long long, long long> Evolve(
    const std::unordered_map<long long, long long>& cs) {
    std::unordered_map<long long, long long> ds;
    for (const auto& [x, c] : cs) {
        if (x == 0) {
            ds[1] += c;
        } else {
            std::string s = std::to_string(x);
            if (s.length() % 2 == 0) {
                ds[std::stoll(s.substr(0, s.length() / 2))] += c;
                ds[std::stoll(s.substr(s.length() / 2, s.length() / 2))] += c;
            } else {
                ds[x * 2024] += c;
            }
        }
    }
    return ds;
}

int main() {
    std::string input = Trim(GetContents("input.txt"));
    std::vector<long long> xs = ParseVector<long long>(input);

    std::unordered_map<long long, long long> cs;
    for (long long x : xs) {
        cs[x]++;
    }

    for (int i = 0; i < 75; i++) {
        cs = Evolve(cs);
    }

    long long answer = 0;
    for (const auto &[_, c] : cs) {
        answer += c;
    }
    std::cout << answer << std::endl;
    return 0;
}