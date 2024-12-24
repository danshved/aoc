#include <algorithm>
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
#include "grid.h"
#include "lex.h"
#include "numbers.h"
#include "parse.h"

int main() {
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), '\n');
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

            Coord delta = b - a;
            int d = Gcd(delta.i, delta.j);
            delta = Coord{delta.i / d, delta.j / d};

            for (Coord cur = a; InBounds(cur, size_i, size_j); cur += delta) {
                antinodes.insert(cur);
            }
            for (Coord cur = a; InBounds(cur, size_i, size_j); cur -= delta) {
                antinodes.insert(cur);
            }
        }
    }

    std::cout << antinodes.size() << std::endl;
    return 0;
}