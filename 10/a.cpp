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

int count = 0;
std::vector<std::vector<bool>> visited;

struct Dir {
    int di;
    int dj;
};
Dir dirs[4] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

void Visit(int i, int j) {
    visited[i][j] = true;
    if (input[i][j] == '9') {
        count++;
    }
    for (int dir = 0; dir < 4; dir++) {
        int i1 = i + dirs[dir].di;
        int j1 = j + dirs[dir].dj;
        if (i1 < 0 || i1 >= height || j1 < 0 || j1 >= width) {
            continue;
        }
        if (input[i1][j1] != input[i][j] + 1) {
            continue;
        }
        if (!visited[i1][j1]) {
            Visit(i1, j1);
        }
    }
}

int main() {
    input = Split(Trim(GetContents("input.txt")), '\n');
    height = input.size();
    width = input[0].size();

    int answer = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (input[i][j] != '0') {
                continue;
            }
            visited.assign(height, std::vector<bool>(width, false));
            count = 0;
            Visit(i, j);
            answer += count;
        }
    }

    std::cout << answer << std::endl;
    return 0;
}