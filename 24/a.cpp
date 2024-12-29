#include <iostream>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

#include "parse.h"

struct Expr {
    std::string left;
    std::string op;
    std::string right;
};

int main() {
    auto [top, bottom] = Split2(Split(Trim(GetContents("input.txt")), "\n"), {""});

    std::unordered_map<std::string, int> values;
    for (const std::string& s : top) {
        auto [l, r] = SplitN(s, ":");
        values[l] = std::stoi(Trim(r));
    }

    std::unordered_map<std::string, Expr> exprs;
    for (const std::string& s : bottom) {
        auto [left, op, right, output] = SplitN(s, " ", " ", " -> ");
        exprs[output] = {left, op, right};
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