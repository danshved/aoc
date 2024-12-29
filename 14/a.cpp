#include <iostream>
#include <string>
#include <vector>

#include "grid.h"
#include "parse.h"

const int kMaxT = 100;
const int kI = 103, kJ = 101;
const Box kBox = {0, 0, kI, kJ};
const Box kBoxes[4] = {
    {0, 0, kI / 2, kJ / 2},
    {0, kJ - kJ / 2, kI / 2, kJ / 2},
    {kI - kI / 2, 0, kI / 2, kJ / 2},
    {kI - kI / 2, kJ - kJ / 2, kI / 2, kJ / 2},
};

PosDir ParseRobot(const std::string& s) {
    auto [_, pj, pi, vj, vi] = SplitN(s, "p=", ",", " v=", ",");
    return {{std::stoi(pi), std::stoi(pj)}, {std::stoi(vi), std::stoi(vj)}};
}

int main() {
    int c[4] = {0, 0, 0, 0};
    for (const std::string& line : Split(Trim(GetContents("input.txt")), "\n")) {
        PosDir robot = ParseRobot(line);
        Coord pos = kBox.Wrap(robot.pos + robot.dir * kMaxT);
        for (int i = 0; i < 4; i++) {
            if (kBoxes[i].contains(pos)) {
                c[i]++;
            }
        }
    }
    std::cout << c[0] * c[1] * c[2] * c[3] << std::endl;
    return 0;
}