#include <iostream>
#include <vector>

#include "order.h"
#include "parse.h"

bool IsSafe(const std::vector<int>& x) {
    if (x[0] == x[1]) {
        return false;
    }
    int mul = Sign(x[1] - x[0]);
    for (int i = 0; i + 1 < x.size(); ++i) {
        int diff = mul * (x[i + 1] - x[i]);
        if (diff < 1 || diff > 3) {
            return false;
        }
    }
    return true;
}

bool IsAlmostSafe(const std::vector<int>& x) {
    if (IsSafe(x)) {
        return true;
    }
    for (int skip = 0; skip < x.size(); ++skip) {
        std::vector<int> y = x;
        y.erase(y.begin() + skip);
        if (IsSafe(y)) {
            return true;
        }
    }
    return false;
}

int main() {
    int answer = 0;
    for (const std::string& s : Split(Trim(GetContents("input.txt")), "\n")) {
        if (IsAlmostSafe(ParseVector<int>(s))) {
            answer++;
        }
    }
    std::cout << answer << std::endl;
    return 0;
}