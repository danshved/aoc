#include <iostream>
#include <string>
#include <vector>

#include "collections.h"
#include "grid.h"
#include "parse.h"

int main() {
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), "\n");
    Box box = Sizes<2>(input);

    int answer = 0;
    for (Coord u : box) {
        if (std::ranges::all_of(Adj4(u), [&](Coord v) {
                return !box.contains(v) || input[v.i][v.j] > input[u.i][u.j];
            })) {
            answer += input[u.i][u.j] - '0' + 1;
        }
    }
    std::cout << answer << std::endl;
    return 0;
}