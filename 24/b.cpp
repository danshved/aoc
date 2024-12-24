#include <algorithm>
#include <bitset>
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

std::unordered_map<std::string, Expr> exprs;

std::string Name(const std::string& prefix, int i) {
    std::string code = {(char)((i / 10) + '0'), (char)(i % 10 + '0')};
    return prefix + code;
}

bool Test(long long x, long long y) {
    std::unordered_map<std::string, int> values;
    for (int i = 0; i < 45; i++) {
        values[Name("x", i)] = (x >> i) & 1;
        values[Name("y", i)] = (y >> i) & 1;
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

    long long answer = 0, expected = x + y;
    for (int i = 0; i < 46; i++) {
        answer |= (long long)calc(calc, Name("z", i)) << i;
    }
    if (answer == expected) {
        return true;
    }

    std::cout << "x        " << std::bitset<46>(x) << std::endl;
    std::cout << "y        " << std::bitset<46>(y) << std::endl;
    std::cout << "got      " << std::bitset<46>(answer) << std::endl;
    std::cout << "expected " << std::bitset<46>(expected) << std::endl;

    for (int i = 0; i < 46; i++) {
        if ((expected ^ answer) & (1ll << i)) {
            std::cout << "Bit " << i << " got " << ((answer >> i) & 1)
                      << ", expected " << ((expected >> i) & 1) << std::endl;
        }
    }

    std::cout << "Wires with 1: ";
    for (const auto& [k, v] : values) {
        if (v != 0) {
            std::cout << k << " ";
        }
    }
    std::cout << std::endl;

    return false;
}

int GetHighest(const std::string& key) {
    static std::unordered_map<std::string, int> highest;
    if (highest.contains(key)) {
        return highest[key];
    }

    if (key[0] == 'x' || key[0] == 'y') {
        return highest[key] = std::stol(key.substr(1));
    }

    Expr e = exprs[key];
    return highest[key] = std::max(GetHighest(e.left), GetHighest(e.right));
}

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input_fixed.txt")), '\n');
    auto [top, bottom] = Split2(lines, std::string());
    for (const std::string& s : bottom) {
        auto ss = Split(s, ' ');
        exprs[ss[4]] = Expr{ss[0], ss[1], ss[2]};
    }

    // Print which highest bit of input every wire depends on as a sanity check.
    std::ofstream out("output.txt");
    std::vector<std::string> keys;
    for (const auto& [k, _] : exprs) {
        keys.push_back(k);
    }
    std::ranges::sort(keys);
    for (const auto& k : keys) {
        out << k << " (" << GetHighest(k) << ")" << std::endl;
    }

    // Probe with sparse inputs and print debug info when there's a mismatch.
    int i;
    for (i = 0; i < 45; i++) {
        if (!Test(1ll < i, 0)) {
            break;
        }
        if (!Test(0, 1ll << i)) {
            break;
        }
    }
    if (i >= 45) {
        std::cout << "OK" << std::endl;
    }

    return 0;
}