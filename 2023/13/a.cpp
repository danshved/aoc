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

std::vector<std::string> Transpose(const std::vector<std::string>& v) {
    int height = v.size();
    int width = v[0].size();
    std::vector<std::string> result(width, std::string(height, '.'));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            result[j][i] = v[i][j];
        }
    }
    return result;
}

unsigned int ToUInt(const std::string& s) {
    unsigned int result = 0;
    for (char c : s) {
        result <<= 1;
        if (c == '#') {
            result |= 0x1;
        }
    }
    return result;
}

std::optional<int> FindSep(const std::vector<std::string> &par) {
    std::vector<unsigned int> v;
    for (const std::string& s : par) {
        v.push_back(ToUInt(s));
    }

    for (int b = 1; b < v.size(); b++) {
        bool found = true;
        for (int j = 0; b - 1 - j >= 0 && b + j < v.size(); j++) {
            if (v[b - 1 - j] != v[b + j]) {
                found = false;
                break;
            }
        }
        if (found) {
            return b;
        }
    }
    return std::nullopt;
}

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), "\n");
    std::vector<std::vector<std::string>> pars = Split(lines, {""});

    int answer = 0;
    for (const std::vector<std::string>& par : pars) {
        if (std::optional<int> sep = FindSep(par); sep.has_value()) {
            answer += 100 * *sep;
            continue;
        }
        if (std::optional<int> sep = FindSep(Transpose(par)); sep.has_value()) {
            answer += *sep;
            continue;
        }
    }

    std::cout << answer << std::endl;
    return 0;
}