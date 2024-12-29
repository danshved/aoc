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

    std::unordered_map<Coord, int> count;
    int answer = 0;
    DFS<Coord>(
        [&](auto& search) {
            for (Coord u : box) {
                if (input[u.i][u.j] == '0') {
                    search.Look(u);
                    answer += count[u];
                }
            }
        },
        [&](auto& search, Coord u) {
            if (input[u.i][u.j] == '9') {
                count[u] = 1;
            }
            for (Coord v : Adj4(u)) {
                if (box.contains(v) && input[v.i][v.j] == input[u.i][u.j] + 1) {
                    search.Look(v);
                    count[u] += count[v];
                }
            }
        });

    std::cout << answer << std::endl;
    return 0;
}