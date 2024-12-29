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

struct Condition {
    char lhs;
    char sign;
    int rhs;
};

Condition ParseCondition(const std::string& s) {
    assert(s[0] == 'x' || s[0] == 'm' || s[0] == 'a' || s[0] == 's');
    assert(s[1] == '<' || s[1] == '>');
    return {s[0], s[1], std::stoi(s.substr(2))};
}

struct Workflow {
    std::vector<std::pair<Condition, std::string>> rules;
    std::string otherwise;
};

Workflow ParseWorkflow(const std::string& s) {
    Workflow result;
    std::vector<std::string> words = Split(s, ",");
    assert(words.size() >= 1);
    for (int i = 0; i < words.size() - 1; i++) {
        auto [l, r] = SplitN(words[i], ":");
        result.rules.emplace_back(ParseCondition(l), r);
    }
    result.otherwise = words.back();
    return result;
}

using Part = std::unordered_map<char, int>;

Part Update(Part p, char key, int value) {
    p[key] = value;
    return p;
}

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), "\n");
    auto [top, bottom] = Split2(lines, {""});

    std::unordered_map<std::string, Workflow> workflows;
    for (const std::string& line : top) {
        auto [name, wf, _] = SplitN(line, "{", "}");
        workflows[name] = ParseWorkflow(wf);
    }

    auto count = [&, ind = 0](auto& self, Part min, Part max,
                              const std::string& name) mutable -> long long {
        if (min['x'] > max['x'] || min['m'] > max['m'] ||
            min['a'] > max['a'] || min['s'] > max['s']) {
            return 0;
        }
        if (name == "R") {
            return 0;
        }
        if (name == "A") {
            return 1ll * (max['x'] - min['x'] + 1) * (max['m'] - min['m'] + 1) *
                   (max['a'] - min['a'] + 1) * (max['s'] - min['s'] + 1);
        }

        long long result = 0;
        Workflow wf = workflows.at(name);
        for (const auto& [cond, to] : wf.rules) {
            if (cond.sign == '<') {
                result += self(self, min, Update(max, cond.lhs, cond.rhs - 1), to);
                min[cond.lhs] = cond.rhs;
            } else {
                result += self(self, Update(min, cond.lhs, cond.rhs + 1), max, to);
                max[cond.lhs] = cond.rhs;
            }
        }
        result += self(self, min, max, wf.otherwise);
        return result;
    };

    Part min = {{'x', 1}, {'m', 1}, {'a', 1}, {'s', 1}};
    Part max = {{'x', 4000}, {'m', 4000}, {'a', 4000}, {'s', 4000}};
    std::cout << count(count, min, max, "in") << std::endl;
    return 0;
}