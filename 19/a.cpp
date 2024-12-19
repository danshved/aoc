#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <queue>
#include <ranges>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "collections.h"
#include "numbers.h"
#include "order.h"
#include "parse.h"

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), '\n');
    auto [top, queries] = Split2(lines, std::string());
    std::vector<std::string> pieces = Split(top[0], ',');
    for (std::string& s : pieces) {
        s = Trim(s);
    }

    int answer = 0;
    for (const std::string& s : queries) {
        std::vector<bool> d(s.size() + 1, false);
        d[s.size()] = true;
        for (int i = s.size() - 1; i >= 0; i--) {
            for (const std::string& piece : pieces) {
                if (i + piece.size() > s.size()) {
                    continue;
                }
                if (std::equal(piece.begin(), piece.end(), s.begin() + i)) {
                    d[i] = d[i] || d[i + piece.size()];
                    if (d[i]) {
                        break;
                    }
                }
            }
        }
        if (d[0]) {
            answer++;
        }
    }

    std::cout << answer << std::endl;
    return 0;
}