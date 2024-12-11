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

#include "numbers.h"
#include "order.h"
#include "parse.h"

std::vector<long long> Evolve(const std::vector<long long>& xs) {
    std::vector<long long> ys;
    for (long long x : xs) {
        if (x == 0) {
            ys.push_back(1);
        } else {
            std::string s = std::to_string(x);
            if (s.length() % 2 == 0) {
                ys.push_back(std::stoll(s.substr(0, s.length() / 2)));
                ys.push_back(std::stoll(s.substr(s.length() / 2, s.length() / 2)));
            } else {
                ys.push_back(x * 2024);
            }
        }
    }
    return ys;
}

int main() {
    std::string input = Trim(GetContents("input.txt"));
    std::vector<long long> xs = ParseVector<long long>(input);

    for (int i = 0; i < 25; i++) {
        std::cout << i << std::endl;
        xs = Evolve(xs);
    }

    std::cout << xs.size() << std::endl;
    return 0;
}