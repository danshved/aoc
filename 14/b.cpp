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

template<typename S>
void PrintRobots(const std::vector<Robot> robots, S& out) {
    std::vector<std::string> image(kSizeY, std::string(kSizeX, '.'));
    for (const Robot& r : robots) {
        image[r.y][r.x] = '#';
    }
    for (const std::string& s : image) {
        out << s << std::endl;
    }
    out << std::endl;
}

int VarX(const std::vector<Robot>& robots) {
    int sum = 0, sum_sq = 0;
    for (const Robot& r : robots) {
        sum += r.x;
        sum_sq += r.x * r.x;
    }
    return sum_sq * robots.size() - sum * sum;
}

int VarY(const std::vector<Robot>& robots) {
    int sum = 0, sum_sq = 0;
    for (const Robot& r : robots) {
        sum += r.y;
        sum_sq += r.y * r.y;
    }
    return sum_sq * robots.size() - sum * sum;
}

int main() {
    std::vector<std::string> lines = Split(Trim(GetContents("input.txt")), '\n');
    std::vector<Robot> robots;
    for (const std::string& line : lines) {
        robots.push_back(ParseRobot(line));
    }

    std::ofstream out("output.txt");
    int t;
    for (t = 0; t < kSizeX * kSizeY; t++) {
        int var_x = VarX(robots);
        if (var_x < 100000000) {
            out << "Low X variance: " << t << " " << var_x << std::endl;
            PrintRobots(robots, out);
        }

        int var_y = VarY(robots);
        if (var_y < 100000000) {
            out << "Low Y variance: " << t << " " << var_y << std::endl;
            PrintRobots(robots, out);
        }

        if (var_x < 100000000 && var_y < 100000000) {
            break;
        }

        for (Robot& robot : robots) {
            robot.x = Clamp(robot.x + robot.vx, kSizeX);
            robot.y = Clamp(robot.y + robot.vy, kSizeY);
        }
    }

    std::cout << t << std::endl;
    return 0;
}