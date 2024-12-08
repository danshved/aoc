#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <utility>

bool IsMAS(char x, char y, char z) {
    return y == 'A' && (x == 'M' && z == 'S' || x == 'S' && z == 'M');
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
    for (int i = 1; i + 1 < height; ++i) {
        for (int j = 1; j + 1 < width; ++j) {
            if (IsMAS(x[i-1][j-1], x[i][j], x[i+1][j+1]) &&
                    IsMAS(x[i-1][j+1], x[i][j], x[i+1][j-1])) {
                ++count;
            }
        }
    }

    std::cout << count << std::endl;
    return 0;
}