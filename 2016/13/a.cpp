#include <iostream>

#include "graph_search.h"
#include "grid.h"

int Weight(int num) {
    return (num == 0) ? 0 : (1 + Weight(num & (num - 1)));
}

int main() {
    Coord start = {1, 1}, end = {31, 39};
    int seed = 1364;

    auto free = [&](Coord c) {
        if (c.i < 0 || c.j < 0) {
            return false;
        }
        return Weight(c.i * c.i + 3 * c.i + 2 * c.i * c.j + c.j + c.j * c.j + seed) % 2 == 0;
    };
    assert(free(start));

    int answer = 0;
    BFSFrom(start, [&](auto& search, Coord u) {
        if (u == end) {
            answer = search.Depth();
            search.Abort();
            return;
        }
        for (Coord v : Adj4(u)) {
            if (free(v)) {
                search.Look(v);
            }
        }
    });
    std::cout << answer << std::endl;
    return 0;
}