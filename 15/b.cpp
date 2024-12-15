#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <queue>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "collections.h"
#include "numbers.h"
#include "order.h"
#include "parse.h"

struct Dir {
    int di;
    int dj;
};

std::unordered_map<char, Dir> kDirs = {
    {'^', {-1, 0}},
    {'v', {1, 0}},
    {'<', {0, -1}},
    {'>', {0, 1}},
};

int height, width;
bool InBounds(int i, int j) {
    return i >= 0 && i < height && j >= 0 && j < width;
}

void PrintMap(std::vector<std::string> matrix, int i, int j) {
    matrix[i][j] = '@';
    std::cout << FormatVector(matrix, "\n") << std::endl;
}

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), '\n');
    auto [pre_matrix, bottom] = Split2(lines, std::string());
    std::string moves = Concat(bottom);

    std::vector<std::string> matrix;
    for (const std::string& s : pre_matrix) {
        std::string s2;
        for (char c : s) {
            switch (c) {
                case '#':
                    s2 += "##";
                    break;
                case '.':
                    s2 += "..";
                    break;
                case '@':
                    s2 += "@.";
                    break;
                case 'O':
                    s2 += "[]";
                    break;
                default:
                    assert(false);
                    break;
            }
        }
        matrix.push_back(std::move(s2));
    }
    height = matrix.size();
    width = matrix[0].size();

    std::cout << moves.size() << " " << height << " " << width << std::endl;

    int i = 0, j = 0;
    for (i = 0; i < height; i++) {
        std::string::size_type p = matrix[i].find('@');
        if (p != std::string::npos) {
            j = p;
            break;
        }
    }
    assert(i < height);
    matrix[i][j] = '.';

    for (char c : moves) {
        Dir d = kDirs[c];

        // PrintMap(matrix, i, j);
        // std::cout << c << " " << d.di << " " << d.dj << std::endl;

        std::queue<std::pair<int, int>> q;
        std::vector<std::pair<int, int>> to_move;
        std::vector<std::vector<bool>> added(height, std::vector<bool>(width, false));
        bool stuck = false;

        q.push(std::make_pair(i, j));
        // std::cout << "pushed " << i << " " << j << std::endl;
        while (!q.empty()) {
            auto [cur_i, cur_j] = q.front();
            q.pop();
            // std::cout << "popped " << cur_i << " " << cur_j << std::endl;
            int i1 = cur_i + d.di;
            int j1 = cur_j + d.dj;
            // std::cout << "looking at " << i1 << " " << j1 << std::endl;
            // std::cout << "i1, j1 = " << i1 << " " << j1 << std::endl;
            if (!InBounds(i1, j1) || matrix[i1][j1] == '#') {
                stuck = true;
                // std::cout << "stuck" << std::endl;
                break;
            }
            if (matrix[i1][j1] == '.') {
                // std::cout << "void" << std::endl;
                continue;
            }

            if (added[i1][j1]) {
                // std::cout << "old" << std::endl;
                continue;
            }
            q.push(std::make_pair(i1, j1));
            // std::cout << "pushed " << i1 << " " << j1 << std::endl;
            to_move.push_back(std::make_pair(i1, j1));
            added[i1][j1] = true;
            // std::cout << "added " << i1 << " " << j1 << std::endl;
            if (d.di != 0) {
                int i2 = i1;
                assert(matrix[i1][j1] == '[' || matrix[i1][j1] == ']');
                int j2 = (matrix[i1][j1] == '[' ? j1 + 1 : (j1 - 1));
                if (!added[i2][j2]) {
                    q.push(std::make_pair(i2, j2));
                    // std::cout << "pushed " << i2 << " " << j2 << std::endl;

                    to_move.push_back(std::make_pair(i2, j2));
                    added[i2][j2] = true;
                    // std::cout << "added " << i2 << " " << j2 << std::endl;
                }
            }
        }
        if (stuck) {
            continue;
        }

        for (auto it = to_move.rbegin(); it != to_move.rend(); it++) {
            auto [i2, j2] = *it;
            matrix[i2 + d.di][j2 + d.dj] = std::exchange(matrix[i2][j2], '.');
        }
        i += d.di;
        j += d.dj;
    }
    PrintMap(matrix, i, j);

    int answer = 0;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            if (matrix[i][j] == '[') {
                answer += 100 * (i) + (j);
            }
        }
    }

    std::cout << answer << std::endl;
    return 0;
}