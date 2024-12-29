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

    std::unordered_set<Coord> antinodes;
    for (Coord a : box) {
        if (input[a.i][a.j] == '.') {
            continue;
        }
        for (Coord b : box) {
            if (a == b || input[a.i][a.j] != input[b.i][b.j]) {
                continue;
            }
            if (box.contains(2 * a - b)) {
                antinodes.insert(2 * a - b);
            }
            if (box.contains(2 * b - a)) {
                antinodes.insert(2 * b - a);
            }
        }
    }

    std::cout << antinodes.size() << std::endl;
    return 0;
}