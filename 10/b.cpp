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

#include "order.h"
#include "numbers.h"
#include "parse.h"

std::vector<std::string> input;
int height, width;
const long long kInf = std::numeric_limits<long long>::max();

std::vector<std::vector<long long>> counts;

struct Dir {
    int di;
    int dj;
};
Dir dirs[4] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

long long GetCount(int i, int j) {
    if (long long c = counts[i][j]; c != kInf) {
        return c;
    }

    if (input[i][j] == '9') {
        counts[i][j] = 1;
        return counts[i][j];
    }

    counts[i][j] = 0;
    for (int dir = 0; dir < 4; dir++) {
        int i1 = i + dirs[dir].di;
        int j1 = j + dirs[dir].dj;
        if (i1 < 0 || i1 >= height || j1 < 0 || j1 >= width) {
            continue;
        }
        if (input[i1][j1] != input[i][j] + 1) {
            continue;
        }
        counts[i][j] += GetCount(i1, j1);
    }
    return counts[i][j];
}

int main() {
    input = Split(Trim(GetContents("input.txt")), '\n');
    height = input.size();
    width = input[0].size();

    long long answer = 0;
    counts.assign(height, std::vector<long long>(width, kInf));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (input[i][j] == '0') {
                answer += GetCount(i, j);
            }
        }
    }

    std::cout << answer << std::endl;
    return 0;
}