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
    for (const std::string& word : words) {
        state = (word[0] == 'R') ? state.RotateRight() : state.RotateLeft();
        state = {state.pos + state.dir * std::stoi(word.substr(1)), state.dir};
    }
    std::cout << state.pos.Manhattan() << std::endl;
    return 0;
}