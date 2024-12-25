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
#include "graph_search.h"
#include "grid.h"
#include "numbers.h"
#include "order.h"
#include "parse.h"

bool Match(const std::vector<int>& lock, const std::vector<int>& key) {
    assert(lock.size() == 5);
    assert(key.size() == 5);
    for (int i = 0; i < 5; i++) {
        if (lock[i] + key[i] > 5) {
            return false;
        }
    }
    return true;
}

int main() {
    auto pars = Split(Split(Trim(GetContents("input.txt")), '\n'), std::string());

    NestedVector<2, int> locks, keys;
    for (const auto& par : pars) {
        assert(Sizes<2>(par) == std::make_tuple(7, 5));
        std::vector<int> seq;
        for (const std::string& s : Transpose(par)) {
            seq.push_back(std::ranges::count(s, '#') - 1);
        }
        if (par[0][0] == '#') {
            locks.push_back(seq);
        } else {
            keys.push_back(seq);
        }
    }

    int answer = 0;    
    for (const auto& lock : locks) {
        for (const auto& key : keys) {
            if (Match(lock, key)) {
                answer++;
            }
        }
    }
    std::cout << answer << std::endl;

    return 0;
}