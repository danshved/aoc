#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <optional>
#include <ranges>
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

int main() {
    std::vector<std::string> input = Split(GetContents("input.txt"), "\n");
    assert(input.size() == 5);
    
    long long answer = 0;
    for (int col = 0; col < input[4].size(); col++) {
        if (input[4][col] == ' ') {
            continue;
        }
        char op = input[4][col];
        assert (op == '*' || op == '+');
        int width = std::find_if(input[4].begin() + col + 1, input[4].end(), [&](char ch) {return ch != ' ';}) - input[4].begin() - col;

        std::vector<std::string> table(4, std::string(width, ' '));
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < width; j++) {
                table[i][j] = input[i][col + j];
            }
        }

        long long acc = (op == '+') ? 0 : 1;
        for (const std::string& str : table) {
            if (Trim(str) == "") {
                continue;
            }
            long long num = std::stoll(str);
            acc = (op == '+') ? (acc + num) : (acc * num);
        }
        answer += acc;
    }

    std::cout << answer << std::endl;
    return 0;
}