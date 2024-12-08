#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <utility>

int vec[8][2] = {
    {0, 1}, {0, -1}, {1, 0}, {-1, 0},
    {1, 1}, {-1, -1}, {1, -1}, {-1, 1},
};

const char xmas[] = "XMAS";

bool IsXMAS(const std::vector<std::string>& x, int start_i, int start_j, int di, int dj) {
    for (int k = 0; k < 4; k++) {
        int i = start_i + k * di;
        int j = start_j + k * dj;
        if (i < 0 || j < 0 || i >= x.size() || j >= x[0].length()) {
            return false;
        }
        if (x[i][j] != xmas[k]) {
            return false;
        }
    }
    return true;
}

int main() {
    std::vector<std::string> x;
    std::ifstream in;
    std::string current;
    in.open("input.txt");
    while (std::getline(in, current)) {
        x.push_back(std::move(current));
    }
    in.close();

    int width = x[0].length();
    int height = x.size();
    int count = 0;
    for (int start_i = 0; start_i < height; ++start_i) {
        for (int start_j = 0; start_j < width; ++start_j) {
            for (int dir = 0; dir < 8; ++dir) {
                if (IsXMAS(x, start_i, start_j, vec[dir][0], vec[dir][1])) {
                    ++count;
                }
            }
        }
    }

    std::cout << count << std::endl;
    return 0;
}