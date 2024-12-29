#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#include "collections.h"
#include "grid.h"
#include "numbers.h"
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

            Coord delta = b - a;
            int d = Gcd(delta.i, delta.j);
            delta = Coord{delta.i / d, delta.j / d};

            for (Coord cur = a; box.contains(cur); cur += delta) {
                antinodes.insert(cur);
            }
            for (Coord cur = a; box.contains(cur); cur -= delta) {
                antinodes.insert(cur);
            }
        }
    }

    std::cout << antinodes.size() << std::endl;
    return 0;
}