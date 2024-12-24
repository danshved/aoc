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

struct Expr {
    std::string left;
    std::string op;
    std::string right;
};

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), '\n');
    auto [top, bottom] = Split2(lines, std::string());

    std::unordered_map<std::string, int> values;
    for (const std::string& s : top) {
        auto [l, r] = Split2(s, ':');
        values[l] = std::stoi(Trim(r));
    }

    std::unordered_map<std::string, Expr> exprs;
    for (const std::string& s : bottom) {
        auto ss = Split(s, ' ');
        exprs[ss[4]] = Expr{ss[0], ss[1], ss[2]};
    }

    auto calc = [&](auto& self, const std::string& arg) -> int {
        if (values.contains(arg)) {
            return values[arg];
        }
        Expr e = exprs.at(arg);
        int l = self(self, e.left);
        int r = self(self, e.right);
        int result = (e.op == "AND")   ? (l & r)
                     : (e.op == "OR")  ? (l | r)
                     : (e.op == "XOR") ? (l ^ r)
                                       : -1;
        assert(result != -1);
        return values[arg] = result;
    };

    std::vector<std::string> outs;
    for (const std::string& k : exprs | std::views::keys) {
        if (k[0] == 'z') {
            outs.push_back(k);
        }
    }
    std::ranges::sort(outs);

    long long answer = 0;
    for (const std::string& out : outs | std::views::reverse) {
        answer = (answer << 1) | calc(calc, out);
    }
    std::cout << answer << std::endl;
    return 0;
}