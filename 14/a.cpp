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

const int kMaxT = 100;
const int kSizeX = 101;
const int kSizeY = 103;

struct Robot {
    int x;
    int y;
    int vx;
    int vy;
};

Robot ParseRobot(const std::string& s) {
    auto [left, right] = Split2(s, ' ');
    auto [s1, s2] = Split2(left.substr(2), ',');
    auto [s3, s4] = Split2(right.substr(2), ',');
    return Robot{std::stoi(s1), std::stoi(s2), std::stoi(s3), std::stoi(s4)};
}

int Clamp(int c, int limit) {
    c %= limit;
    c += limit;
    return c % limit;
}

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), '\n');

    int ll = 0, lh = 0, hl = 0, hh = 0;
    for (const std::string& line : lines) {
        Robot robot = ParseRobot(line);
        robot.x = Clamp(robot.x + robot.vx * kMaxT, kSizeX);
        robot.y = Clamp(robot.y + robot.vy * kMaxT, kSizeY);

        if (robot.x < kSizeX / 2) {
            if (robot.y < kSizeY / 2) ll++;
            else if (robot.y > kSizeY / 2) lh++;
        } else if (robot.x > kSizeX / 2) {
            if (robot.y < kSizeY / 2) hl++;
            else if (robot.y > kSizeY / 2) hh++;
        }
    }

    std::cout << ll * lh * hl * hh;
    return 0;
}