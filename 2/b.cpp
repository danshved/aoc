#include <iostream>
#include <iterator>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include "parse.h"

bool IsSafe(const std::vector<int>& x) {
    if (x[0] == x[1]) {
        return false;
    }
    int mul = (x[0] > x[1]) ? -1 : 1;
    for(int i = 0; i + 1 < x.size(); ++i) {
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
    for (int skip=0; skip < x.size(); ++skip) {
        std::vector<int> y;
        for (int i = 0; i < x.size(); ++i) {
            if (i != skip) {
                y.push_back(x[i]);
            }
        }
        if (IsSafe(y)) {
            return true;
        }
    }
    return false;
}

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), '\n');
    int answer = 0;
    for (const std::string& s : lines) {
        std::vector<int> xs = ParseVector<int>(s);
        if (IsAlmostSafe(xs)) {
            answer++;
        }
    }
    std::cout << answer << std::endl;
    return 0;
}