#include <iostream>
#include <string>

#include "grid.h"
#include "parse.h"

Coord RotateRight(Coord v, int degrees) {
    assert(degrees % 90 == 0);
    int count = ((degrees / 90) % 4 + 4) % 4;
    for (int i = 0; i < count; i++) {
        v = v.RotateRight();
    }
    return v;
}

int main() {
    PosDir state = {{0, 0}, kEast};
    for (const std::string& line : Split(Trim(GetContents("input.txt")), "\n")) {
        int num = std::stoi(line.substr(1));
        // clang-format off
        switch (line[0]) {
            case 'N': state.pos += num * kNorth; break;
            case 'S': state.pos += num * kSouth; break;
            case 'E': state.pos += num * kEast; break;
            case 'W': state.pos += num * kWest; break;
            case 'F': state.pos += num * state.dir; break;
            case 'R': state.dir = RotateRight(state.dir, num); break;
            case 'L': state.dir = RotateRight(state.dir, -num); break;
            default : assert(false);
        }
        // clang-format on
    }
    std::cout << state.pos.Manhattan() << std::endl;
    return 0;
}