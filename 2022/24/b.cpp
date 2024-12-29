#include <iostream>
#include <string>
#include <vector>

#include "collections.h"
#include "graph_search.h"
#include "grid.h"
#include "numbers.h"
#include "parse.h"

struct State {
    Coord pos;
    int time;

    bool operator==(const State&) const = default;
};

template <>
struct std::hash<State> {
    size_t operator()(const State& s) const {
        return SeqHash(s.pos, s.time);
    }
};

const Coord kMoves[5] = {kSouth, kNorth, kWest, kEast, {0, 0}};

int main() {
    std::vector<std::string> input = Split(Trim(GetContents("input.txt")), "\n");
    Box box = Sizes<2>(input);
    Box small_box = {1, 1, box.size_i - 2, box.size_j - 2};
    Coord start = {0, 1}, end = {box.size_i - 1, box.size_j - 2};

    auto is_occupied = [&](const State& s) {
        if (input[s.pos.i][s.pos.j] == '#') {
            return true;
        }
        if (!small_box.contains(s.pos)) {
            return false;
        }
        return std::ranges::any_of(kDirArrows, [&](const auto& kv) {
            Coord c = small_box.Wrap(s.pos - kv.second * s.time);
            return input[c.i][c.j] == kv.first;
        });
    };

    int answer = 0;
    int time_period = Lcm(small_box.size_i, small_box.size_j);
    State cur_state = {start, 0};
    std::vector<Coord> goals = {end, start, end};
    for (const Coord& goal : goals) {
        std::optional<State> goal_state = std::nullopt;
        BFSFrom(cur_state, [&](auto& search, const State& u) {
            if (u.pos == goal) {
                answer += search.Depth();
                goal_state = u;
                search.Abort();
                return;
            }
            for (const Coord& move : kMoves) {
                State v = {u.pos + move, (u.time + 1) % time_period};
                if (box.contains(v.pos) && !is_occupied(v)) {
                    search.Look(v);
                }
            }
        });
        cur_state = goal_state.value();
    }
    std::cout << answer << std::endl;
    return 0;
}