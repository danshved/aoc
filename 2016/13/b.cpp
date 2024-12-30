#include <iostream>
#include <unordered_set>

#include "graph_search.h"
#include "grid.h"

int Weight(int num) {
    return (num == 0) ? 0 : (1 + Weight(num & (num - 1)));
}

int main() {
    Coord start = {1, 1};
    int seed = 1364;

    auto free = [&](Coord c) {
        if (c.i < 0 || c.j < 0) {
            return false;
        }
        return Weight(c.i * c.i + 3 * c.i + 2 * c.i * c.j + c.j + c.j * c.j + seed) % 2 == 0;
    };
    assert(free(start));

    std::unordered_set<Coord> reached;
    BFSFrom(start, [&](auto& search, Coord u) {
        if (search.Depth() > 50) {
            search.Abort();
            return;
        }
        reached.insert(u);
        for (Coord v : Adj4(u)) {
            if (free(v)) {
                search.Look(v);
            }
        }
    });
    std::cout << reached.size() << std::endl;
    return 0;
}