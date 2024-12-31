#include <iostream>
#include <string>
#include <vector>

#include "grid.h"
#include "parse.h"

int main() {
    Box box(6, 40);
    std::vector<std::string> bitmap(box.size_i, std::string(box.size_j, ' '));

    auto [top, bottom] = Split2(Split(Trim(GetContents("input.txt")), "\n"), {""});
    for (const std::string& line : top) {
        auto [l, r] = SplitN(line, ",");
        int x = std::stoi(l), y = std::stoi(r);
        for (const std::string& line : bottom) {
            auto [_, axis, val_s] = SplitN(line, "fold along ", "=");
            int val = std::stoi(val_s);
            if (axis == "x") {
                if (x > val) {
                    x = 2 * val - x;
                }
            } else {
                assert(axis == "y");
                if (y > val) {
                    y = 2 * val - y;
                }
            }
        }
        assert(box.contains({y, x}));
        bitmap[y][x] = '#';
    }
    std::cout << FormatVector(bitmap, "\n") << std::endl;
    return 0;
}