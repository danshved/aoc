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
    auto [top, bottom] = Split2(lines, std::string());

    std::unordered_map<int, std::unordered_set<int>> edges;
    for (const std::string& s : top) {
        auto [l, r] = Split2(s, '|');
        edges[std::stoi(l)].insert(std::stoi(r));
    }

    int answer = 0;
    for (const std::string& s : bottom) {
        std::vector<int> pages = ParseVector<int>(s);
        bool ok = true;
        for (int i = 0; i < pages.size() && ok; i++) {
            for (int j = i + 1; j < pages.size() && ok; j++) {
                ok = ok && !edges[pages[j]].contains(pages[i]);
            }
        }

        if (ok) {
            answer += pages[pages.size() / 2];
        }
    }

    std::cout << answer << std::endl;
    return 0;
}