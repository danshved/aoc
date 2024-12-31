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
#include <cassert>

#include "order.h"
#include "numbers.h"
#include "parse.h"

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), "\n");
    std::vector<std::vector<std::string>> pars = Split(lines, {""});
    assert(pars.size() == 2);
    assert(pars[0].size() == 1);

    std::string seq = pars[0][0];
    std::unordered_map<std::string, std::pair<std::string, std::string>> pairs;
    for (const std::string& s : pars[1]) {
        std::string k = s.substr(0, 3);
        pairs[k] = std::make_pair(s.substr(7,3), s.substr(12, 3));
        assert(s.length() == 16);
    }

    std::string s = "AAA";
    int count = 0;
    int i = 0;
    for (; s != "ZZZ"; count++) {
        char c = seq[i++];
        i %= seq.length();
        s = (c == 'L') ? pairs[s].first : pairs[s].second;
    }

    std::cout << count << "\n";
    return 0;
}