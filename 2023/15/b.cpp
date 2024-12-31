#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <list>
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
#include "graph_search.h"
#include "grid.h"
#include "numbers.h"
#include "order.h"
#include "parse.h"

struct Entry {
    std::string label;
    int value;
};

int Hash(const std::string& s) {
    int x = 0;
    for (char c : s) {
        x += (int)(unsigned char)c;
        x *= 17;
        x &= 0xff;
    }
    return x;
}

int main() {
    std::vector<std::string> ss = Split(Trim(GetContents("input.txt")), ",");
    std::vector<std::list<Entry>> boxes(256, std::list<Entry>());

    for (const std::string& s : ss) {
        if (s.back() == '-') {
            std::string label = s.substr(0, s.size() - 1);
            int h = Hash(label);
            auto it = std::ranges::find_if(boxes[h], [&](const Entry& e) {
                return e.label == label;
            });
            if (it != boxes[h].end()) {
                boxes[h].erase(it);
            }
        } else {
            assert(s[s.size() - 2] == '=');
            Entry new_e = {
                .label = s.substr(0, s.size() - 2),
                .value = s.back() - '0',
            };
            int h = Hash(new_e.label);
            auto it = std::ranges::find_if(boxes[h], [&](const Entry& e) {
                return e.label == new_e.label;
            });
            if (it != boxes[h].end()) {
                *it = new_e;
            } else {
                boxes[h].push_back(new_e);
            }
        }
    }

    int answer = 0;
    for (int h = 0; h < 256; h++) {
        int slot = 1;
        for (const Entry& e : boxes[h]) {
            answer += (h + 1) * slot * e.value;
            slot++;
        }
    }
    std::cout << answer << std::endl;
    return 0;
}