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

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), '\n');
    auto [matrix, bottom] = Split2(lines, std::string());
    std::string moves = Concat(bottom);
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
    std::cout << i << " " << j << std::endl;

    for (char c : moves) {
        // auto m2 = matrix;
        // m2[i][j] = '@';
        // std::cout << FormatVector(m2, "\n") << std::endl;

        Dir d = kDirs[c];
        // std::cout << c << " " << d.di << " " << d.dj << std::endl;
        int k = 1;
        for (; InBounds(i + k * d.di, j + k * d.dj) && matrix[i + k * d.di][j + k * d.dj] == 'O'; k++);
        if (!InBounds(i + k * d.di, j + k * d.dj) || matrix[i + k * d.di][j + k * d.dj] != '.') {
            continue;
        }
        matrix[i + k * d.di][j + k * d.dj] = 'O';
        matrix[i + d.di][j + d.dj] = '.';
        i += d.di;
        j += d.dj;
    }
    // auto m2 = matrix;
    // m2[i][j] = '@';
    // std::cout << FormatVector(m2, "\n") << std::endl;

    int answer = 0;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            if (matrix[i][j] == 'O') {
                answer += 100 * (i) + (j);
            }
        }
    }

    std::cout << answer << std::endl;
    return 0;
}