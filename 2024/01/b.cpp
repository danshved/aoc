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
    auto x = xs.begin();
    auto y = ys.begin();
    while (x != xs.end() && y != ys.end()) {
        if (*x < *y) {
            x++;
        } else if (*x > *y) {
            y++;
        } else {
            auto x1 = std::find_if(x, xs.end(), [&](int t) {return t != *x;});
            auto y1 = std::find_if(y, ys.end(), [&](int t) {return t != *y;});
            answer += *x * (x1 - x) * (y1 - y);
            x = x1;
            y = y1;
        }
    }

    std::cout << answer << std::endl;
    return 0;
}