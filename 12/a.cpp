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

int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

std::vector<std::string> input;
int width, height;

std::vector<std::vector<bool>> visited;
int vertices, edges;

void Visit(int i, int j) {
    vertices++;
    visited[i][j] = true;
    for (int dir = 0; dir < 4; dir++) {
        int i1 = i + dirs[dir][0];
        int j1 = j + dirs[dir][1];
        if (i1 < 0 || i1 >= height || j1 < 0 || j1 >= width || input[i][j] != input[i1][j1]) {
            continue;
        }

        edges++;
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
    visited.assign(height, std::vector<bool>(width, false));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (visited[i][j]) {
                continue;
            }
            
            vertices = 0;
            edges = 0;
            Visit(i, j);
            int perimeter = 4 * vertices - edges;
            answer += vertices * perimeter;
        }
    }

    std::cout << answer << std::endl;
    return 0;
}