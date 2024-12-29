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

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), '\n');

    long long answer = 0;
    for (const std::string& line : lines) {
        auto [s, right] = Split2(line, ' ');
        std::vector<int> segs2 = ParseVector<int>(right);

        s = s + "?" + s + "?" + s + "?" + s + "?" + s;
        std::vector<int> segs;
        for (int k = 0; k < 5; k++) {
            std::copy(segs2.begin(), segs2.end(), std::back_inserter(segs));
        }

        std::vector<std::vector<long long>> d(s.size() + 1, std::vector<long long>(segs.size() + 1, 0));
        d[s.size()][segs.size()] = 1;

        for (int i = s.size() - 1; i >= 0; i--) {
            for (int j = segs.size(); j >= 0; j--) {
                if (s[i] == '.' || s[i] == '?') {
                    d[i][j] += d[i + 1][j];
                }

                if (j == segs.size()) {
                    continue;
                }
                if (i + segs[j] > s.size()) {
                    continue;
                }

                auto begin = s.begin() + i;
                auto end = begin + segs[j];
                if (std::find(begin, end, '.') != end) {
                    continue;
                }
                if (end != s.end() && *end == '#') {
                    continue;
                }

                d[i][j] += (end == s.end()) ? d[s.size()][j + 1] :
                    d[i + segs[j] + 1][j + 1];
            }
        }

        answer += d[0][0];
    }

    std::cout << answer << std::endl;
    return 0;
}