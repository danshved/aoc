#include <algorithm>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

#include "collections.h"
#include "grid.h"
#include "parse.h"

int main() {
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), "\n");
    Box box = Sizes<2>(input);
    int answer = 0;

    while (true) {
        int removed = 0;
        for (Coord u : box) {
            if (input[u.i][u.j] != '@') {
                continue;
            }
            int count = std::ranges::count_if(Adj8(u), [&](Coord v) {
                return box.contains(v) && input[v.i][v.j] == '@';
            });
            if (count < 4) {
                input[u.i][u.j] = '.';
                answer++;
                removed++;
            }
        }
        if (removed == 0) {
            break;
        }
    }
    std::cout << answer << std::endl;

    return 0;
}