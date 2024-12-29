#include <iostream>
#include <string>
#include <vector>

#include "grid.h"
#include "parse.h"

const Box kBox = {103, 101};

PosDir ParseRobot(const std::string& s) {
    auto [_, pj, pi, vj, vi] = SplitN(s, "p=", ",", " v=", ",");
    return {{std::stoi(pi), std::stoi(pj)}, {std::stoi(vi), std::stoi(vj)}};
}

template<typename S>
void PrintRobots(const std::vector<PosDir>& robots, S& out) {
    std::vector<std::string> image(kBox.size_i, std::string(kBox.size_j, '.'));
    for (const PosDir& r : robots) {
        image[r.pos.i][r.pos.j] = '#';
    }
    out << FormatVector(image, "\n") << std::endl;
}

int VarI(const std::vector<PosDir>& robots) {
    int sum = 0, sum_sq = 0;
    for (const PosDir& r : robots) {
        sum += r.pos.i;
        sum_sq += r.pos.i * r.pos.i;
    }
    return sum_sq * robots.size() - sum * sum;
}

int VarJ(const std::vector<PosDir>& robots) {
    int sum = 0, sum_sq = 0;
    for (const PosDir& r : robots) {
        sum += r.pos.j;
        sum_sq += r.pos.j * r.pos.j;
    }
    return sum_sq * robots.size() - sum * sum;
}


int main() {
    std::vector<PosDir> robots;
    for (const std::string& line : Split(Trim(GetContents("input.txt")), "\n")) {
        robots.push_back(ParseRobot(line));
    }

    std::ofstream out("output.txt");
    int t;
    for (t = 0; t < kBox.size_i * kBox.size_j; t++) {
        int var_i = VarI(robots);
        if (var_i < 100000000) {
            out << "Low I variance: " << t << " " << var_i << std::endl;
            PrintRobots(robots, out);
        }

        int var_j = VarJ(robots);
        if (var_j < 100000000) {
            out << "Low J variance: " << t << " " << var_j << std::endl;
            PrintRobots(robots, out);
        }

        if (var_i < 100000000 && var_j < 100000000) {
            break;
        }

        for (PosDir& robot : robots) {
            robot.pos = kBox.Wrap(robot.pos + robot.dir);
        }
    }

    std::cout << t << std::endl;
    return 0;
}