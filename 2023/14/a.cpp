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

#include "numbers.h"
#include "order.h"
#include "parse.h"

int main() {
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), '\n');
    int height = input.size();
    int width = input[0].size();

    int answer = 0;
    for (int j = 0; j < width; j++) {
        std::cout << j << ": ";
        std::vector<int> q;
        int q_index = 0;
        for (int i = 0; i < height; i++) {
            if (input[i][j] != '#') {
                q.push_back(i);
            } else {
                q.clear();
                q_index = 0;
            }
            if (input[i][j] == 'O') {
                int weight = height - q[q_index++];
                std::cout << weight << " ";
                answer += weight;
            }
        }
        std::cout << std::endl;
    }
    std::cout << answer << std::endl;
    return 0;
}