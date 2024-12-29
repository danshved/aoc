#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#include "collections.h"
#include "grid.h"
#include "parse.h"

int main() {
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), "\n");
    auto [size_i, size_j] = Sizes<2>(input);

    std::unordered_set<Coord> antinodes;
    for (Coord a : Bounds(size_i, size_j)) {
        if (input[a.i][a.j] == '.') {
            continue;
        }
        for (Coord b : Bounds(size_i, size_j)) {
            if (a == b || input[a.i][a.j] != input[b.i][b.j]) {
                continue;
            }
            if (InBounds(2 * a - b, size_i, size_j)) {
                antinodes.insert(2 * a - b);
            }
            if (InBounds(2 * b - a, size_i, size_j)) {
                antinodes.insert(2 * b - a);
            }
        }
    }

    std::cout << antinodes.size() << std::endl;
    return 0;
}