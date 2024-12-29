#include <iostream>
#include <string>
#include <vector>

#include "collections.h"
#include "graph_search.h"
#include "grid.h"
#include "parse.h"

int main() {
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), "\n");
    Box box = Sizes<2>(input);

    int answer = 0;
    for (Coord start : box) {
        if (input[start.i][start.j] != '0') {
            continue;
        }
        DFSFrom(start, [&](auto& search, Coord u) {
            if (input[u.i][u.j] == '9') {
                answer++;
            }
            for (Coord v : Adj4(u)) {
                if (box.contains(v) && input[v.i][v.j] == input[u.i][u.j] + 1) {
                    search.Look(v);
                }
            }
        });
    }

    std::cout << answer << std::endl;
    return 0;
}