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

int dirs[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

std::vector<std::string> input;
int width, height;

std::vector<std::vector<bool>> visited;
int vertices;
std::vector<std::vector<std::vector<bool>>> fence;
std::vector<std::vector<std::vector<bool>>> visited_fence;

void Visit(int i, int j) {
    vertices++;
    visited[i][j] = true;
    for (int dir = 0; dir < 4; dir++) {
        int i1 = i + dirs[dir][0];
        int j1 = j + dirs[dir][1];
        if (i1 < 0 || i1 >= height || j1 < 0 || j1 >= width || input[i][j] != input[i1][j1]) {
            fence[i][j][dir] = true;
        } else if (!visited[i1][j1]) {
            Visit(i1, j1);
        }
    }
}

void VisitFence(int i, int j, int dir) {
    visited_fence[i][j][dir] = true;
    for (int d = 1; d < 4; d += 2) {
        int dir2 = (dir + d) % 4;
        int i1 = i + dirs[dir2][0];
        int j1 = j + dirs[dir2][1];
        if (i1 < 0 || i1 >= height || j1 < 0 || j1 >= width || !fence[i1][j1][dir]) {
            continue;
        }
        if (!visited_fence[i1][j1][dir]) {
            VisitFence(i1, j1, dir);
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
            fence.assign(height, std::vector<std::vector<bool>>(width, std::vector<bool>(4, false)));
            Visit(i, j);

            int sides = 0;
            visited_fence.assign(height, std::vector<std::vector<bool>>(width, std::vector<bool>(4, false)));
            for (int i1 = 0; i1 < height; i1++) {
                for (int j1 = 0; j1 < width; j1++) {
                    for (int dir = 0; dir < 4; dir++) {
                        if (!fence[i1][j1][dir]) {
                            continue;
                        }
                        if (!visited_fence[i1][j1][dir]) {
                            VisitFence(i1, j1, dir);
                            sides++;
                        }
                    }
                }
            }

            answer += vertices * sides;
        }
    }

    std::cout << answer << std::endl;
    return 0;
}