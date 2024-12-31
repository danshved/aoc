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

Part ParsePart(const std::string& part_s) {
    auto [_, x, m, a, s, _2] = SplitN(part_s, "{x=", ",m=", ",a=", ",s=", "}");
    return {{'x', std::stoi(x)}, {'m', std::stoi(m)}, {'a', std::stoi(a)}, {'s', std::stoi(s)}};
}

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), "\n");
    auto [top, bottom] = Split2(lines, {""});

    std::unordered_map<std::string, Workflow> workflows;
    for (const std::string& line : top) {
        auto [name, wf, _] = SplitN(line, "{", "}");
        workflows[name] = ParseWorkflow(wf);
    }

    int answer = 0;
    for (const std::string& line : bottom) {
        Part part = ParsePart(line);
        Workflow wf = workflows.at("in");
        while (true) {
            auto it = std::ranges::find_if(wf.rules, [&](const auto& rule) {
                const Condition& cond = rule.first;
                return (cond.sign == '<') ? part[cond.lhs] < cond.rhs
                                          : part[cond.lhs] > cond.rhs;
            });
            std::string name = (it != wf.rules.end()) ? it->second : wf.otherwise;
            if (name == "A") {
                answer += part['x'] + part['m'] + part['a'] + part['s'];
                break;
            } else if (name == "R") {
                break;
            } else {
                if (!workflows.contains(name)) {
                }
                wf = workflows.at(name);
            }
        }
    }

    std::cout << answer << std::endl;
    return 0;
}