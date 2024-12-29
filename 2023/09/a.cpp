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

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), '\n');

    long long answer = 0;
    for (const std::string& line : lines) {
        std::vector<long long> xs = ParseVector<long long>(line);
        long long y = 0;
        while(!xs.empty()) {
            y += xs.back();
            std::vector<long long> ds(xs.size() - 1);
            for (int i = 0; i < ds.size(); i++) {
                ds[i] = xs[i + 1] - xs[i];
            }
            xs = std::move(ds);
        }
        std::cout << y << std::endl;
        answer += y;
    }

    std::cout << answer << std::endl;
    return 0;
}