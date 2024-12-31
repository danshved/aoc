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

    std::unordered_set<Coord> east, south;
    for (Coord c : box) {
        if (input[c.i][c.j] == '>') {
            east.insert(c);
        } else if (input[c.i][c.j] == 'v') {
            south.insert(c);
        }
    }

    auto move = [&](std::unordered_set<Coord> &set, Coord dir) {
        std::unordered_set<Coord> new_set;
        bool moved = false;
        for (Coord u : set) {
            Coord v = box.Wrap(u + dir);
            if (!east.contains(v) && !south.contains(v)) {
                new_set.insert(v);
                moved = true;
            } else {
                new_set.insert(u);
            }
        }
        set = std::move(new_set);
        return moved;
    };

    int step = 0;
    for (;; step++) {
        // Use & instead of && to avoid short-curcuit evaluation.
        if (!move(east, kEast) & !move(south, kSouth)) {
            break;
        }
    }
    std::cout << step + 1 << std::endl;
    return 0;
}