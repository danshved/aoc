#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#include "collections.h"
#include "grid.h"
#include "parse.h"

int main() {
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), "\n");
    Box box = Sizes<2>(input);
    PosDir guard = {FindOrDie<2>(input, '^'), kNorth};

    std::unordered_set<Coord> visited;
    while (true) {
        visited.insert(guard.pos);
        PosDir next = guard.Step();
        if (!box.contains(next.pos)) {
            break;
        } else if (input[next.pos.i][next.pos.j] == '#') {
            guard = guard.RotateRight();
        } else {
            guard = next;
        }
    }

    std::cout << visited.size() << std::endl;
    return 0;
}