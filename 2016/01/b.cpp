#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <queue>
#include <ranges>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "collections.h"
#include "graph_search.h"
#include "grid.h"
#include "numbers.h"
#include "order.h"
#include "parse.h"

int main() {
    std::vector<std::string> words = Split(Trim(GetContents("input.txt")), ", ");
    PosDir state = {{0, 0}, kNorth};
    std::unordered_set<Coord> visited;
    visited.insert(state.pos);
    for (const std::string& word : words) {
        state = (word[0] == 'R') ? state.RotateRight() : state.RotateLeft();
        int steps = std::stoi(word.substr(1));
        bool found = false;
        for (int step = 0; step < steps && !found; step++) {
            state = state.Step();
            if (auto [_, inserted] = visited.insert(state.pos); !inserted) {
                found = true;
            }
        }
        if (found) {
            break;
        }
    }
    std::cout << state.pos.Manhattan() << std::endl;
    return 0;
}