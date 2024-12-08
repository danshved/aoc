#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <optional>
#include <algorithm>
#include <utility>
#include <limits>

#include "parse.h"
#include "lex.h"

int gcd(int a, int b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        a %= b;
        std::swap(a, b);
    }
    return a;
}

int main() {
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), '\n');
    int height = input.size();
    int width = input[0].size();

    std::vector<std::vector<bool>> antinode(height, std::vector<bool>(width, false));
    for (int i1 = 0; i1 < height; i1++) {
        for (int j1 = 0; j1 < width; j1++) {
            if (input[i1][j1] == '.') {
                continue;
            }
            for (int i2 = 0; i2 < height; i2++) {
                for (int j2 = 0; j2 < width; j2++) {
                    if (input[i2][j2] != input[i1][j1]) {
                        continue;
                    }
                    if (i1 == i2 && j1 == j2) {
                        continue;
                    }

                    int di = i1 - i2;
                    int dj = j1 - j2;
                    int t = gcd(di, dj);
                    di /= t;
                    dj /= t;

                    for (int t = 0; ; t++) {
                        int i3 = i1 + t * di;
                        int j3 = j1 + t * dj;
                        if (i3 >= 0 && i3 < height && j3 >= 0 && j3 < width) {
                            antinode[i3][j3] = true;
                        } else {
                            break;
                        }
                    }

                    for (int t = 0; ; t++) {
                        int i3 = i1 - t * di;
                        int j3 = j1 - t * dj;
                        if (i3 >= 0 && i3 < height && j3 >= 0 && j3 < width) {
                            antinode[i3][j3] = true;
                        } else {
                            break;
                        }
                    }
                }
            }
        }
    }

    int answer = 0;
    for (const std::vector<bool>& v : antinode) {
        answer += std::count(v.begin(), v.end(), true);
    }

    std::cout << answer << std::endl;
    return 0;
}