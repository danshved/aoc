#include <algorithm>
#include <iostream>
#include <vector>

#include "parse.h"

int main() {
    std::vector<int> xs, ys;
    for (const std::string line : Split(Trim(GetContents("input.txt")), "\n")) {
        auto [a, b] = SplitN(line, " ");
        xs.push_back(std::stoi(a));
        ys.push_back(std::stoi(b));
    }

    int answer = 0;
    std::sort(xs.begin(), xs.end());
    std::sort(ys.begin(), ys.end());
    for (int i = 0; i < xs.size(); i++) {
        answer += std::abs(xs[i] - ys[i]);
    }

    std::cout << answer << std::endl;
    return 0;
}