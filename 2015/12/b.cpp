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

template <typename I>
std::tuple<I, int, bool> Value(I, I);

template <typename I>
std::tuple<I, int, bool> Array(I cur, I end) {
    int acc = 0;

    assert(cur != end);
    assert(*cur == '[');
    cur++;

    while (true) {
        auto [next, val, _] = Value(cur, end);
        acc += val;
        cur = next;

        assert(cur != end);
        if (*cur == ']') {
            cur++;
            break;
        } else {
            assert(*cur == ',');
            cur++;
        }
    }
    return {cur, acc, false};
}

template <typename I>
std::tuple<I, int, bool> Object(I cur, I end) {
    int acc = 0;
    bool acc_red = false;

    assert(cur != end);
    assert(*cur == '{');
    cur++;

    while (true) {
        cur = std::find(cur, end, ':');
        assert(cur != end);
        cur++;

        auto [next, val, red] = Value(cur, end);
        acc += val;
        acc_red |= red;
        cur = next;

        assert(cur != end);
        if (*cur == '}') {
            cur++;
            break;
        } else {
            assert(*cur == ',');
            cur++;
        }
    }
    return {cur, acc_red ? 0 : acc, false};
}

template <typename I>
std::tuple<I, int, bool> String(I cur, I end) {
    bool red = false;

    assert(cur != end);
    assert(*cur == '\"');
    cur++;

    I next = std::find(cur, end, '\"');
    assert(next != end);
    if (std::string(cur, next) == "red") {
        red = true;
    }

    cur = next;
    cur++;
    return {cur, 0, red};
}

template <typename I>
std::tuple<I, int, bool> Int(I cur, I end) {
    int sign = 1;
    assert(cur != end);
    if (*cur == '-') {
        sign = -1;
        cur++;
    }

    I next = std::find_if(cur, end, [](char c) { return !isdigit(c); });
    assert(next != cur);
    int result = sign * std::stoi(std::string(cur, next));
    cur = next;
    return {cur, result, false};
}

template <typename I>
std::tuple<I, int, bool> Value(I cur, I end) {
    assert(cur != end);
    if (*cur == '[') {
        return Array(cur, end);
    } else if (*cur == '{') {
        return Object(cur, end);
    } else if (*cur == '\"') {
        return String(cur, end);
    } else {
        return Int(cur, end);
    }
}

int main() {
    std::string input = Trim(GetContents("input.txt"));
    auto [it, answer, _] = Value(input.begin(), input.end());
    assert(it == input.end());

    std::cout << answer << std::endl;
    return 0;
}