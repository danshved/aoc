#include <iostream>
#include <string>
#include <vector>

#include "collections.h"
#include "grid.h"
#include "parse.h"

bool IsMAS(char x, char y, char z) {
    return y == 'A' && (x == 'M' && z == 'S' || x == 'S' && z == 'M');
}

int main() {
    std::vector<std::string> x = Split(Trim(GetContents("input.txt")), "\n");
    auto [size_i, size_j] = Sizes<2>(x);

    int answer = 0;
    for (int i = 1; i + 1 < size_i; ++i) {
        for (int j = 1; j + 1 < size_j; ++j) {
            if (IsMAS(x[i - 1][j - 1], x[i][j], x[i + 1][j + 1]) &&
                IsMAS(x[i - 1][j + 1], x[i][j], x[i + 1][j - 1])) {
                ++answer;
            }
        }
    }

    std::cout << answer << std::endl;
    return 0;
}